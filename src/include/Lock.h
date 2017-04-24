//! @file
//! @brief Lockを制御するクラス
//! @author Koichi NABETANI
//! @date 2017/04/24
#ifndef LOCK_H
#define LOCK_H

#include <boost/thread.hpp>

namespace realcore
{
// Lockを行うかのフラグ
constexpr bool kLockFree = false;   // Lockを行わない
constexpr bool kLockTable = true;   // Lockを行う

// 前方宣言
class LockTest;

class Lock
{
  friend class LockTest;

public:
  Lock(boost::mutex * const mutex, const bool lock_control)
  : mutex_(nullptr), lock_control_(lock_control)
  {
    if(lock_control){
      mutex_ = mutex;
      mutex->lock();
    } 
  }

  void Unlock(){
    if(lock_control_){
      assert(mutex_ != nullptr);
      mutex_->unlock();
    }
  }

  ~Lock()
  {
    Unlock();
  }

private:
  boost::mutex *mutex_;
  const bool lock_control_;
};  // class Lock

}   // namespace realcore

#endif    // LOCK_H
