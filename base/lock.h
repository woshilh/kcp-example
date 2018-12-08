#ifndef LOCK_H_
#define LOCK_H_
namespace zsy{
class Lock{
public:
    virtual void Enter()=0;
    virtual void Leave()=0;
    virtual ~Lock(){}
};
class LockScope{
public:
    explicit LockScope(Lock *lock);
    ~LockScope();
private:
    Lock *lock_;
};
}
#endif // LOCK_H_
