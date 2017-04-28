#include <string>
#include <thread>

#include <boost/thread.hpp>

#include "gtest/gtest.h"

#include "SearchManager.h"

using namespace std;

namespace realcore
{

void InterruptWait();

class SearchManagerTest
: public ::testing::Test
{
public:
  void DefaultConstructorTest(){
    SearchManager search_manager(kNoInterruptionPoint);

    ASSERT_EQ(0, search_manager.node_);
    ASSERT_EQ(0, search_manager.node_limit_);

    auto elapsed_time = chrono::system_clock::now() - search_manager.search_start_time_;
    auto elapsed_time_ms = chrono::duration_cast<chrono::milliseconds>(elapsed_time).count();
    ASSERT_GE(10, elapsed_time_ms);

    ASSERT_EQ(0, search_manager.elapsed_time_limit_);
    ASSERT_FALSE(search_manager.catch_interrupt_exception_);
    ASSERT_FALSE(search_manager.interruption_);
  }

  void SetNodeLimitTest()
  {
    SearchManager search_manager(kNoInterruptionPoint);

    search_manager.SetNodeLimit(1);
    ASSERT_EQ(1, search_manager.node_limit_);
  }

  void SetSearchTimeLimitTest()
  {
    SearchManager search_manager(kNoInterruptionPoint);

    search_manager.SetSearchTimeLimit(1000);
    ASSERT_EQ(1000, search_manager.elapsed_time_limit_);
  }

  void SearchStartTest()
  {
    SearchManager search_manager(kNoInterruptionPoint);
    auto prev_time = search_manager.search_start_time_;

    this_thread::sleep_for(chrono::milliseconds(10));
    search_manager.SearchStart();

    auto elapsed_time = search_manager.search_start_time_ - prev_time;
    auto elapsed_time_ms = chrono::duration_cast<chrono::milliseconds>(elapsed_time).count();
    ASSERT_NE(0, elapsed_time_ms);
  }
};

TEST_F(SearchManagerTest, DefaultConstructorTest)
{
  DefaultConstructorTest();
}

TEST_F(SearchManagerTest, AddGetNodeTest)
{
  SearchManager search_manager(kNoInterruptionPoint);
  ASSERT_EQ(0, search_manager.GetNode());
  
  search_manager.AddNode();
  ASSERT_EQ(1, search_manager.GetNode());
}

TEST_F(SearchManagerTest, AddGetProofTreeTest)
{
  SearchManager search_manager(kNoInterruptionPoint);

  ASSERT_EQ(0, search_manager.GetProofTreeCount());
  ASSERT_EQ(0, search_manager.GetProofTreeSuccessCount());
  
  search_manager.AddGetProofTreeResult(true);

  ASSERT_EQ(1, search_manager.GetProofTreeCount());
  ASSERT_EQ(1, search_manager.GetProofTreeSuccessCount());

  search_manager.AddGetProofTreeResult(false);

  ASSERT_EQ(2, search_manager.GetProofTreeCount());
  ASSERT_EQ(1, search_manager.GetProofTreeSuccessCount());
}

TEST_F(SearchManagerTest, AddGetSimulationTest)
{
  SearchManager search_manager(kNoInterruptionPoint);

  ASSERT_EQ(0, search_manager.GetSimulationCount());
  ASSERT_EQ(0, search_manager.GetSimulationSuccessCount());

  search_manager.AddSimulationResult(true);

  ASSERT_EQ(1, search_manager.GetSimulationCount());
  ASSERT_EQ(1, search_manager.GetSimulationSuccessCount());

  search_manager.AddSimulationResult(false);

  ASSERT_EQ(2, search_manager.GetSimulationCount());
  ASSERT_EQ(1, search_manager.GetSimulationSuccessCount());
}

TEST_F(SearchManagerTest, SetNodeLimitTest)
{
  SetNodeLimitTest();
}

TEST_F(SearchManagerTest, SetSearchTimeLimitTest)
{
  SetSearchTimeLimitTest();
}

TEST_F(SearchManagerTest, SearchStartTest)
{
  SearchStartTest();
}

TEST_F(SearchManagerTest, IsTerminateNodeTest)
{
  SearchManager search_manager(kNoInterruptionPoint);
  
  ASSERT_FALSE(search_manager.IsTerminate());

  search_manager.AddNode();   // node: 1
  ASSERT_FALSE(search_manager.IsTerminate());
  
  search_manager.SetNodeLimit(2);
  ASSERT_FALSE(search_manager.IsTerminate());

  search_manager.AddNode();   // node: 2
  ASSERT_FALSE(search_manager.IsTerminate());
  
  search_manager.AddNode();   // node: 3
  ASSERT_TRUE(search_manager.IsTerminate());
}

TEST_F(SearchManagerTest, IsTerminateTimeTest)
{
  SearchManager search_manager(kNoInterruptionPoint);
  
  ASSERT_FALSE(search_manager.IsTerminate());
  this_thread::sleep_for(chrono::milliseconds(10));

  ASSERT_FALSE(search_manager.IsTerminate());

  search_manager.SetSearchTimeLimit(30);
  search_manager.SearchStart();
  
  this_thread::sleep_for(chrono::milliseconds(10));
  ASSERT_FALSE(search_manager.IsTerminate());
  
  this_thread::sleep_for(chrono::milliseconds(30));
  ASSERT_TRUE(search_manager.IsTerminate());
}

void InterruptWait()
{
  SearchManager search_manager(kCatchInterruptException);

  while(!search_manager.IsTerminate()){
  }
}

TEST_F(SearchManagerTest, IsTerminateInterruptionTest)
{
  boost::thread thread_interrupt_wait(&InterruptWait);

  thread_interrupt_wait.interrupt();
  thread_interrupt_wait.join();
}

TEST_F(SearchManagerTest, GetSearchTimeTest)
{
  SearchManager search_manager(kNoInterruptionPoint);
  this_thread::sleep_for(chrono::milliseconds(10));
  
  const auto ms_time = search_manager.GetSearchTime();
  ASSERT_NEAR(10, ms_time, 3);
}

}   // namespace realcore
