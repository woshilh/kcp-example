#ifndef RTC_BASE_PLATFORM_THREAD_TYPES_H_
#define RTC_BASE_PLATFORM_THREAD_TYPES_H_
#include <pthread.h>
#include <unistd.h>
namespace rtc {
typedef pthread_t PlatformThreadId;
typedef pthread_t PlatformThreadRef;

// Retrieve the ID of the current thread.
PlatformThreadId CurrentThreadId();

// Retrieves a reference to the current thread. On Windows, this is the same
// as CurrentThreadId. On other platforms it's the pthread_t returned by
// pthread_self().
PlatformThreadRef CurrentThreadRef();

// Compares two thread identifiers for equality.
bool IsThreadRefEqual(const PlatformThreadRef& a, const PlatformThreadRef& b);
}
#endif
