#ifndef ENTITY_KCPSTREAM_H_
#define ENTITY_KCPSTREAM_H_
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include "taskqueue.h"
class KcpStream{
public:
	KcpStream(rtc::Ns3TaskQueue* w);
	~KcpStream();
	void EnableRecord(std::string name);
	bool IsSessionStop();
	bool HasDateToSend();
	uint32_t RequestData(void *buf,uint32_t buf_len);
	void MessageFromRemoteEnd(void *buf,uint32_t buf_len);
	uint32_t GetTotal(){
		return total_;
	}
	void RecordData(uint32_t len);
private:
	void ParseHeader();
	uint32_t BufRead(uint8_t*buf,uint32_t len);
	void CloseRecord();
	void LogToFile(const void *data);
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
	bool log_enable_{false};
	std::fstream f_record_;
	rtc::Ns3TaskQueue *w_{NULL};
	uint32_t first_recv_ts_{0};
};
#endif /* ENTITY_KCPSTREAM_H_ */
