#include "platform_thread_types.h"
namespace rtc{
PlatformThreadId CurrentThreadId() {
return reinterpret_cast<pthread_t>(pthread_self());
}
PlatformThreadRef CurrentThreadRef() {
return pthread_self();
}
bool IsThreadRefEqual(const PlatformThreadRef& a, const PlatformThreadRef& b) {
return pthread_equal(a, b);
}
}


