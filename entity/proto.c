#include "proto.h"
#include <stdlib.h>
int encode_length(uint8_t *buf,uint32_t buf_len,uint32_t msglen){
	int ret=-1;
	uint8_t temp[4];
	char first=0;
	int64_t next=msglen;
	uint8_t key=0;
	if(next){
		do{
			first=next%128;
			next=next/128;
			temp[key]=first;
			if(next>0){
				temp[key]|=128;
			}
			key++;
		}while(next>0&&key<=4);
		if(next>0){
            printf("overfolw\n");
		}else{
			if(key<=buf_len){
				ret=key;
				memcpy(buf,temp,ret);
			}
		}

	}
	return ret;
}



