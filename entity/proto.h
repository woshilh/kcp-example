#ifndef ENTITY_PROTO_H_
#define ENTITY_PROTO_H_
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
enum Proto_Type{
	pt_data,
	pt_signal,
};
typedef struct{
uint8_t type;
uint32_t length;
}message_t;
#define KCP_MTU 1400
int encode_length(uint8_t *buf,uint32_t buf_len,uint32_t msglen);
#ifdef __cplusplus
}
#endif
#endif /* ENTITY_PROTO_H_ */
