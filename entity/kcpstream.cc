#include "kcpstream.h"
#include "kcp_utils.h"
#include "proto.h"
#include <string.h>
static uint32_t max_buf_size=1000;
static char terminate_signal[]="stop";
KcpStream::KcpStream(){
	memset(first_buf,0,max_buf_size);
}
bool KcpStream::IsSessionStop(){
	uint32_t now=iclock();
    if(error_){
        session_stop_=true;
    }
	if(recv_done_){
		if(now>=(done_ts_+max_wait_)){
			session_stop_=true;
		}
	}
	return session_stop_;
}
bool KcpStream::HasDateToSend(){
	bool ret=false;
	if(recv_done_&&!terminal_sent_){
		ret=true;
	}
	return ret;
}
uint32_t KcpStream::RequestData(void *buf,uint32_t buf_len){
    uint32_t ret=0;
	uint8_t *w_ptr=(uint8_t*)buf;
	message_t message;
	message.type=pt_signal;
	message.length=strlen(terminate_signal);
	uint8_t varint[4];
	int ret_enc=0;
	ret_enc=encode_length(varint,4,message.length);
	memcpy(w_ptr,&message.type,1);
	w_ptr+=1;
	memcpy(w_ptr,varint,ret_enc);
	w_ptr+=ret_enc;
	memcpy(w_ptr,terminate_signal,message.length);
	ret=1+ret_enc+message.length;
	terminal_sent_=true;
	return ret;
}
void KcpStream::ParseHeader(){
	uint32_t length=0;
	uint8_t type=0;
	length=BufRead(&type,1);
	uint8_t byte=0;
	uint32_t remain=0;
	uint32_t remain_multi=1;
	uint32_t c=0;
	do{
		length=BufRead(&byte,1);
		if(length==1){
			remain+=(byte&127)*remain_multi;
			remain_multi*=128;
			c++;
			if(c>4){
				error_=true;
				break;
			}
		}else{
			error_=true;
			break;
		}
	}while(byte&128);
	if(!error_){
		header_parsed_=true;
		total_=1+c+remain;
        printf("total%d\n",total_);
	}
}
uint32_t KcpStream::BufRead(uint8_t*buf,uint32_t len){
	uint32_t ret=0;
	uint32_t remain=max_buf_size-buf_offset_;
	if(remain>0){
		ret=len>remain?remain:len;
		memcpy(buf,&first_buf[buf_offset_],ret);
		buf_offset_+=ret;
	}
	return ret;
}
//assume first packet longer enough to get the message length info
void KcpStream::MessageFromRemoteEnd(void *buf,uint32_t buf_len){
	if(error_||buf_len==0){
		return ;
	}
	recved_+=buf_len;
	if(!header_read_done_){
		uint32_t min_length=buf_len>max_buf_size?max_buf_size:buf_len;
		memcpy(first_buf,buf,min_length);
		ParseHeader();
		header_read_done_=true;
	}
	if(header_parsed_){
        printf("recv %d\n",recved_);
		if(recved_==total_){
            recv_done_=true;
			done_ts_= iclock();
		}
	}
}




