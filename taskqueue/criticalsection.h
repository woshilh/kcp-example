/*
 *  Copyright 2004 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef RTC_BASE_CRITICALSECTION_H_
#define RTC_BASE_CRITICALSECTION_H_
#include "platform_thread_types.h"
namespace rtc {

// Locking methods (Enter, TryEnter, Leave)are const to permit protecting
// members inside a const context without requiring mutable CriticalSections
// everywhere.
class  CriticalSection {
 public:
  CriticalSection();
  ~CriticalSection();

  void Enter() const;
  bool TryEnter() const;
  void Leave() const;

 private:
  // Use only for RTC_DCHECKing.
  bool CurrentThreadIsOwner() const;
  mutable pthread_mutex_t mutex_;
};
// CritScope, for serializing execution through a scope.
class  CritScope {
 public:
  explicit CritScope(const CriticalSection* cs);
  ~CritScope();

 private:
  const CriticalSection* const cs_;
};
}
#endif
