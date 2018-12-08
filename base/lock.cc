#include "lock.h"
namespace zsy{
LockScope::LockScope(Lock *lock):lock_(lock){
    lock_->Enter();
}
LockScope::~LockScope(){
    lock_->Leave();
}
}
