#include "proto.h"
#include "fakefilereader.h"
#include "kcpstream.h"
#include <stdio.h>
#include <memory.h>
#include "kcp_utils.h"
int main(){
    uint32_t msg_len=1024;
    int ret=0;
    FakeFileReader reader;
    reader.SetDataSize(1500);
    uint32_t buf_len=1000;
    uint8_t buf[buf_len];
    int read_ret=0;
    int read_c=0;
    KcpStream stream(NULL);
    do{
        memset(buf,0,buf_len);
        read_ret=reader.RequestData(buf,buf_len);
        if(read_ret>0){
        //stream.MessageFromRemoteEnd(buf,read_ret);
        read_c+=read_ret;
        }

    }while(read_ret);
    uint32_t recv=0;
    recv=stream.GetTotal();
    printf("%d,%d\n",read_c,recv);
    return ret;
}
