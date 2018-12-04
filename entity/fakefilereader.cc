#include "fakefilereader.h"
#include "proto.h"
#include <assert.h>
#include <memory.h>
static uint32_t file_size=10*1024*1024;//10MB
FakeFileReader::FakeFileReader(){
}
void FakeFileReader::SetDataSize(uint32_t len){
	if(total_==0){
		total_=len;
	}
}
void FakeFileReader::MessageFromRemoteEnd(void *buf,uint32_t buf_len){
	if(!recv_terminate_){
		uint8_t *r_ptr=(uint8_t *)buf;
		uint8_t byte=r_ptr[0];
		if(byte==pt_signal){
			recv_terminate_=true;
			session_stop_=true;
		}
	}
}
uint32_t FakeFileReader::RequestData(void *buf,uint32_t buf_len){
	uint32_t ret=0;
	if(total_==0){
		total_=file_size;
	}
	uint32_t payload_size=0;
	if(wirte_c_<total_){
		if(wirte_c_==0){
            uint8_t *w_ptr=(uint8_t*)buf;
			message_t message;
			message.type=pt_data;
			message.length=total_;
			uint8_t varint[4];
			int ret_enc=0;
			ret_enc=encode_length(varint,4,total_);
			assert(ret_enc>0);
            if(total_<KCP_MTU){
                payload_size=total_;
            }else{
                payload_size=KCP_MTU-(sizeof(uint8_t)+ret_enc);
            }
			memcpy(w_ptr,&message.type,1);
			w_ptr+=1;
			memcpy(w_ptr,varint,ret_enc);
            ret=1+ret_enc+payload_size;
		}else{
			uint32_t remain=total_-wirte_c_;
			if(remain>KCP_MTU){
				payload_size=KCP_MTU;
			}else{
				payload_size=remain;
			}
			ret=payload_size;
		}
		wirte_c_+=payload_size;
	}
	return ret;
}



