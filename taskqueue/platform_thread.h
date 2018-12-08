/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef RTC_BASE_PLATFORM_THREAD_H_
#define RTC_BASE_PLATFORM_THREAD_H_

#include <string>
#include "platform_thread_types.h"
namespace rtc {
typedef void (*ThreadRunFunction)(void*);
enum ThreadPriority {
  kLowPriority = 1,
  kNormalPriority = 2,
  kHighPriority = 3,
  kHighestPriority = 4,
  kRealtimePriority = 5
};
class PlatformThread {
 public:
  PlatformThread(ThreadRunFunction func,
                 void* obj,
                 const char* thread_name,
                 ThreadPriority priority = kNormalPriority);
  virtual ~PlatformThread();

  const std::string& name() const { return name_; }
  void Start();

  bool IsRunning() const;

  // Returns an identifier for the worker thread that can be used to do
  // thread checks.
  PlatformThreadRef GetThreadRef() const;

  // Stops (joins) the spawned thread.
  void Stop();
    // Set the priority of the thread. Must be called when thread is running.
  // TODO(tommi): Make private and only allow public support via ctor.
 private:
  void Run();
  ThreadRunFunction const run_function_ = nullptr;
  const ThreadPriority priority_ = kNormalPriority;
  void* const obj_;
  // TODO(pbos): Make sure call sites use string literals and update to a const
  // char* instead of a std::string.
  const std::string name_;
  static void* StartThread(void* param);
  pthread_t thread_ = 0;
};
}
#endif
