#include "criticalsection.h"
namespace rtc{
CriticalSection::CriticalSection() {
  pthread_mutexattr_t mutex_attribute;
  pthread_mutexattr_init(&mutex_attribute);
  pthread_mutexattr_settype(&mutex_attribute, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&mutex_, &mutex_attribute);
  pthread_mutexattr_destroy(&mutex_attribute);
}
CriticalSection::~CriticalSection(){
pthread_mutex_destroy(&mutex_);
}
void CriticalSection::Enter()const{
  pthread_mutex_lock(&mutex_);
}
bool CriticalSection::TryEnter()const{
  if (pthread_mutex_trylock(&mutex_)!= 0)
    return false;
}
void CriticalSection::Leave()const{
pthread_mutex_unlock(&mutex_);
}
CritScope::CritScope(const CriticalSection* cs) : cs_(cs) { cs_->Enter(); }
CritScope::~CritScope() { cs_->Leave(); }
}
