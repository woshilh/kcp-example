#include "platform_thread.h"
namespace{
struct ThreadAttributes {
  ThreadAttributes() { pthread_attr_init(&attr); }
  ~ThreadAttributes() { pthread_attr_destroy(&attr); }
  pthread_attr_t* operator&() { return &attr; }
  pthread_attr_t attr;
};
}
namespace rtc{
void* PlatformThread::StartThread(void* param) {
  static_cast<PlatformThread*>(param)->Run();
  return 0;
}
void PlatformThread::Start() {
  ThreadAttributes attr;
  // Set the stack stack size to 1M.
  pthread_attr_setstacksize(&attr, 1024 * 1024);
  pthread_create(&thread_, &attr, &StartThread, this);
}
bool PlatformThread::IsRunning() const {
 return thread_ != 0;
}
PlatformThreadRef PlatformThread::GetThreadRef() const {
return thread_;
}
void PlatformThread::Stop() {
pthread_join(thread_, nullptr);
thread_=0;
}
void PlatformThread::Run() {
  if (run_function_) {
    run_function_(obj_);
    return;
  }
}
PlatformThread::PlatformThread(ThreadRunFunction func,
                               void* obj,
                               const char* thread_name,
                               ThreadPriority priority /*= kNormalPriority*/)
    : run_function_(func), priority_(priority), obj_(obj), name_(thread_name) {
}
PlatformThread::~PlatformThread() {}
}
