#include <boost/thread.hpp>

#include "SearchManager.h"

using namespace std;
using namespace realcore;

const bool SearchManager::IsTerminate()
{
  if(node_limit_ != 0 && node_ > node_limit_){
    return true;
  }

  if(elapsed_time_limit_ != 0){
    auto elapsed_time_ms = GetSearchTime();

    if(elapsed_time_ms > elapsed_time_limit_){
      return true;
    }
  }

  if(interruption_){
    return true;
  }

  try{
    if(catch_interrupt_exception_){
      boost::this_thread::interruption_point();
    }
  }catch(boost::thread_interrupted){
    interruption_ = true;
    return true;
  }

  return false;
}

SearchCounter SearchManager::GetSearchTime() const
{
  auto elapsed_time = chrono::system_clock::now() - search_start_time_;
  auto elapsed_time_ms = chrono::duration_cast<chrono::milliseconds>(elapsed_time).count();
  
  return elapsed_time_ms;
}
