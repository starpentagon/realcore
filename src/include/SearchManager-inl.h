#ifndef SEARCH_MANAGER_INL_H
#define SEARCH_MANAGER_INL_H

#include "SearchManager.h"

namespace realcore{

inline SearchManager::SearchManager(const bool catch_interrupt_exception)
: node_(0), node_limit_(0), search_start_time_(std::chrono::system_clock::now()), elapsed_time_limit_(0), 
  get_proof_tree_(0), get_proof_tree_success_(0), simulation_(0), simulation_success_(0),
  catch_interrupt_exception_(catch_interrupt_exception), interruption_(false)
{
}

inline void SearchManager::AddNode()
{
  ++node_;
}

inline SearchCounter SearchManager::GetNode() const
{
  return node_;
}

inline void SearchManager::AddGetProofTreeResult(const bool is_generated)
{
  ++get_proof_tree_;

  if(is_generated){
    ++get_proof_tree_success_;
  }
}

inline SearchCounter SearchManager::GetProofTreeCount() const
{
  return get_proof_tree_;
}

inline SearchCounter SearchManager::GetProofTreeSuccessCount() const
{
  return get_proof_tree_success_;
}

inline void SearchManager::AddSimulationResult(const bool is_simulation_success)
{
  ++simulation_;

  if(is_simulation_success){
    ++simulation_success_;
  }
}

inline SearchCounter SearchManager::GetSimulationCount() const
{
  return simulation_;
}

inline SearchCounter SearchManager::GetSimulationSuccessCount() const
{
  return simulation_success_;
}

inline void SearchManager::SetNodeLimit(const SearchCounter node_limit)
{
  node_limit_ = node_limit;
}

inline void SearchManager::SetSearchTimeLimit(const SearchCounter elapsed_time_limit)
{
  elapsed_time_limit_ = elapsed_time_limit;
}

inline void SearchManager::SearchStart()
{
  search_start_time_ = std::chrono::system_clock::now();
}

}   // namespace realcore

#endif    // SEARCH_MANAGER_INL_H
