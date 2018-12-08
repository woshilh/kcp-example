#include "cf_platform.h"
#include "kcpstream.h"
#include "kcp_utils.h"
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <memory.h>
static char serv_ip[]="10.0.4.2";

uint16_t serv_port=4321;

static int run_status=1;
void signal_exit_handler(int sig)
{
	run_status=0;
}
su_socket fd;
su_addr remote_addr;
int udp_output(const char *buf, int len, ikcpcb *kcp, void *user)
{
    char send_buf[2000];
    memcpy(send_buf,buf,len);
	su_udp_send(fd,&remote_addr,send_buf,len);
	return 0;
}
int main(){
    signal(SIGTERM, signal_exit_handler);
	signal(SIGINT, signal_exit_handler);
	signal(SIGTSTP, signal_exit_handler);
    su_udp_create(serv_ip,serv_port,&fd);

    KcpStream stream;
	ikcpcb *kcp=ikcp_create(0x11223344, (void*)(&stream));

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

	int buf_len=1500;
	char buf[1500]={0};
	int kcp_recv_buf_len=1500;
	char kcp_recv_buf[1500];

	uint32_t current = iclock();
	uint32_t slap = current + 20;
	int hr=0;
    int ret=0;
    while(!stream.IsSessionStop()&&run_status){
    	current = iclock();
    	ikcp_update(kcp, iclock());
        memset(buf,0,buf_len);
        ret=su_udp_recv(fd,&remote_addr,buf,buf_len,0);
        if(ret>0){
            //printf("recv\n");
        	ikcp_input(kcp,buf,ret);
        }
        if(stream.HasDateToSend()){
        	uint32_t req_len=0;
    		for (; current >= slap; slap += 20) {
    			memset(buf,0,buf_len);
    			req_len=stream.RequestData(buf,buf_len);
    			// 发送上层协议包
    			ikcp_send(kcp,buf,req_len);
    			ikcp_flush(kcp);
    		}
        }
        while(true){
        	hr=ikcp_recv(kcp,kcp_recv_buf,kcp_recv_buf_len);
        	if(hr<0)
        		break;
        	stream.MessageFromRemoteEnd((uint8_t*)kcp_recv_buf,(uint32_t)hr);
        }
    }
    ikcp_release(kcp);
    su_socket_destroy(fd);
}
