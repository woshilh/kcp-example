#ifndef ENTITY_FAKEFILEREADER_H_
#define ENTITY_FAKEFILEREADER_H_
#include<stdint.h>
class FakeFileReader{
public:
	FakeFileReader();
	~FakeFileReader(){}
	void SetDataSize(uint32_t len);
	void MessageFromRemoteEnd(void *buf,uint32_t buf_len);
	uint32_t RequestData(void *buf,uint32_t buf_len);
	bool IsSessionStop(){
		return session_stop_;
	}
	bool IsReadStop(){
		return read_stop_;
	}
private:
	bool session_stop_{false};
	bool read_stop_{false};
	uint32_t total_{0};
	uint32_t wirte_c_{0};
	bool recv_terminate_{false};
};




#endif /* ENTITY_FAKEFILEREADER_H_ */
