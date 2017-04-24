#include "gtest/gtest.h"

#include "Lock.h"

namespace realcore
{
class LockTest
: public ::testing::Test
{
public:
  void LockFreeConstructorTest(){
    Lock lock(nullptr, kLockFree);

    ASSERT_EQ(nullptr, lock.mutex_);
    ASSERT_EQ(kLockFree, lock.lock_control_);
  }

  void LockConstructorTest(){
    boost::mutex mutex;
    Lock lock(&mutex, kLockTable);

    ASSERT_EQ(&mutex, lock.mutex_);
    ASSERT_EQ(kLockTable, lock.lock_control_);
  }
};

TEST_F(LockTest, LockFreeConstructorTest)
{
  LockFreeConstructorTest();
}

TEST_F(LockTest, LockConstructorTest)
{
  LockConstructorTest();
}

}   // namespace realcore
