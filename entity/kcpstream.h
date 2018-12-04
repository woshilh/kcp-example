#ifndef ENTITY_KCPSTREAM_H_
#define ENTITY_KCPSTREAM_H_
#include <stdint.h>
class KcpStream{
public:
	KcpStream();
	~KcpStream(){};
	bool IsSessionStop();
	bool HasDateToSend();
	uint32_t RequestData(void *buf,uint32_t buf_len);
	void MessageFromRemoteEnd(void *buf,uint32_t buf_len);
	uint32_t GetTotal(){
		return total_;
	}
private:
	void ParseHeader();
	uint32_t BufRead(uint8_t*buf,uint32_t len);
	bool session_stop_{false};
	bool recv_done_{false};
	uint32_t recved_{0};
	uint32_t total_{0};
	uint32_t done_ts_{0};
	uint32_t max_wait_{1000};
	uint8_t first_buf[1000];
	bool header_parsed_{false};
	bool header_read_done_{false};
	bool error_{false};
	bool terminal_sent_{false};
	uint32_t buf_offset_{0};
};
#endif /* ENTITY_KCPSTREAM_H_ */
