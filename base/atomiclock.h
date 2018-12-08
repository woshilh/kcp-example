#ifndef _ATOMIC_LOCK_H_
#define _ATOMIC_LOCK_H_
/*
int compare_and_swap ( int *memory_location, int expected_value, int new_value)
{
  int old_value = *memory_location;
  if (old_value == expected_value)
     *memory_location = new_value;
  return old_value;
}
*/
#include "lock.h"
#include<atomic>
namespace zsy{
class AtomicLock:public Lock{
public:
    AtomicLock():mutex_(0),lock_(0),unlock_(1){}
    ~AtomicLock(){}
    void Enter() override{
        while(!__sync_bool_compare_and_swap (&mutex_,lock_, 1)){

        }
    }
    void Leave() override{
        __sync_bool_compare_and_swap (&mutex_,unlock_, 0) ;
    }
private:
    int mutex_;
    int lock_;
    int unlock_;
};
class SpinLock:public Lock{
public:
    SpinLock():flag_(ATOMIC_FLAG_INIT){}
    ~SpinLock(){}
    void Enter() override{
        while(flag_.test_and_set(std::memory_order_acquire));
    }
    void Leave() override{
        flag_.clear(std::memory_order_release);
    }
private:
    std::atomic_flag flag_;
};
}
#endif // _ATOMIC_LOCK_H_
