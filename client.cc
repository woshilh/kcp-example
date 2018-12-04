#include "cf_platform.h"
#include "ikcp.h"
#include "kcp_utils.h"
#include "proto.h"
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "fakefilereader.h"
static char ip[]="127.0.0.1";
uint16_t client_port=1234;

static char addr[]="127.0.0.1:4321";
static int run_status=1;
void signal_exit_handler(int sig)
{
	run_status=0;
}
su_socket fd;
su_addr serv_addr;
int udp_output(const char *buf, int len, ikcpcb *kcp, void *user)
{
    char send_buf[2000];
    memcpy(send_buf,buf,len);
	su_udp_send(fd,&serv_addr,send_buf,len);
	return 0;
}
uint32_t get_total_length(uint32_t data_size){
        uint32_t ret=0;
		uint8_t varint[4];
		int ret_enc=0;
		ret_enc=encode_length(varint,4,data_size);
        ret=sizeof(uint8_t)+ret_enc+data_size;
        return  ret;        
}
int main(){
    signal(SIGTERM, signal_exit_handler);
	signal(SIGINT, signal_exit_handler);
	signal(SIGTSTP, signal_exit_handler);
	uint32_t msg_len=1024*1024*8;//1MB //10*1024*1024;
	FakeFileReader reader;
	reader.SetDataSize(msg_len);
	ikcpcb *kcp=ikcp_create(0x11223344, (void*)(&reader));
	kcp->output=udp_output;
	int mode=0;
	// 判断测试用例的模式
	if (mode == 0) {
		// 默认模式
		ikcp_nodelay(kcp, 0, 10, 0, 0);
	}
	else if (mode == 1) {
		// 普通模式，关闭流控等
		ikcp_nodelay(kcp, 0, 10, 0, 1);
	}	else {
		// 启动快速模式
		// 第二个参数 nodelay-启用以后若干常规加速将启动
		// 第三个参数 interval为内部处理时钟，默认设置为 10ms
		// 第四个参数 resend为快速重传指标，设置为2
		// 第五个参数 为是否禁用常规流控，这里禁止
		ikcp_nodelay(kcp, 1, 10, 2, 1);
		kcp->rx_minrto = 10;
		kcp->fastresend = 1;
	}
    su_udp_create(ip,client_port,&fd);
    su_string_to_addr(&serv_addr,addr);
    int buf_len=1500;
    char buf[1500]={0};
    su_addr remote_addr;
    int ret=0;
    int recv_buf_len=1500;
    char recv_buf[recv_buf_len];
	uint32_t current = iclock();
	uint32_t slap = current + 20;
	int hr=0;
    uint32_t encode_size=get_total_length(msg_len);
    printf("total %d\n",encode_size);
    while(!reader.IsSessionStop()&&run_status){
    	current = iclock();
    	ikcp_update(kcp, iclock());
        memset(buf,0,buf_len);
        ret=su_udp_recv(fd,&remote_addr,buf,buf_len,0);
        if(ret>0){
            //printf("recv\n");
        	ikcp_input(kcp,buf,ret);
        }
        if(!reader.IsReadStop()){
        	uint32_t req_len=0;
    		for (; current >= slap; slap += 20) {
    			memset(buf,0,buf_len);
    			req_len=reader.RequestData(buf,buf_len);
                if(req_len>0){
                    ikcp_send(kcp,buf,req_len);
                }
    			
    		}
        }

        while(true){
        	hr=ikcp_recv(kcp,recv_buf,recv_buf_len);
        	if(hr<0)
        		break;
        	reader.MessageFromRemoteEnd((uint8_t*)recv_buf,(uint32_t)hr);
        }

    }
    ikcp_release(kcp);
    su_socket_destroy(fd);
}
