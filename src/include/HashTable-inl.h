#ifndef HASH_TABLE_INL_H
#define HASH_TABLE_INL_H
#include <iostream>
#include "HashTable.h"

namespace realcore{

template<class T>
HashTable<T>::HashTable(const size_t table_space, const bool lock_control)
: lock_control_(lock_control), logic_counter_(0)
{
  const size_t table_size = CalcHashTableSize(table_space);
  hash_table_.reserve(table_size);
  
  for(size_t i=0; i<table_size; i++){
    hash_table_.emplace_back();
  }

  mutex_list_.reserve(table_size);

  if(lock_control){
    // 要素ごとのmutexを生成する
    for(size_t i=0; i<table_size; i++){
      mutex_list_.emplace_back(new boost::mutex);
    }
  }else{
    mutex_list_.assign(table_size, nullptr);
  }
}

template<class T>
HashTable<T>::~HashTable()
{
  for(const auto mutex_ptr : mutex_list_)
  {
    delete mutex_ptr;
  }
}

template<class T>
inline const size_t HashTable<T>::GetTableIndex(const HashValue hash_value) const
{
  return hash_value % hash_table_.size();
}

template<class T>
const bool HashTable<T>::find(const HashValue hash_value, T * const element) const
{
  const auto index = GetTableIndex(hash_value);
  const T& hash_data = hash_table_[index];

  if(hash_data.hash_value != hash_value || hash_data.logic_counter != logic_counter_){
    return false;
  }

  // Hash値が一致するデータがある
  assert(element != nullptr);

  *element = hash_data;
  return true;
}

template<class T>
void HashTable<T>::Upsert(const HashValue hash_value, const T &element)
{
  const auto index = GetTableIndex(hash_value);
  hash_table_[index] = element;
}

template<class T>
void HashTable<T>::LogicalInitialize()
{
  constexpr TableLogicCounter kMaxCounter = std::numeric_limits<TableLogicCounter>::max();

  if(logic_counter_ == kMaxCounter){
    // カウンタが上限まできている場合は物理クリアを行う
    clear();
    return;
  }

  ++logic_counter_;
}

template<class T>
void HashTable<T>::clear()
{
  const auto table_size = hash_table_.size();

  hash_table_.clear();
  hash_table_.reserve(table_size);
  
  for(size_t i=0; i<table_size; i++){
    hash_table_.emplace_back();
  }

  logic_counter_ = 0;
}

template<class T>
inline constexpr size_t HashTable<T>::CalcHashTableSize(const size_t table_space){
  constexpr size_t kMegaBytes = 1024 * 1024;
  const size_t max_element_count = table_space * kMegaBytes / (sizeof(T) + sizeof(boost::mutex*));

  // @see doc/08_hash_value/prime.xlsx
  constexpr size_t table_size[] = 
  {
    1610612711,
    805306357,
    402653117,
    201326557,
    100663291,
    50331599,
    25165813,
    12582893,
    6291449,
    3145721,
    1572853,
    786431,
    393209,
    196597,
    98299,
    49139,
    24571,
    12281,
    6143,
    3067,
    1531,
  };

  constexpr size_t kTableSize = 21;

  for(size_t i=0; i<kTableSize; i++){
    if(table_size[i] <= max_element_count){
      return table_size[i];
    }
  }

  return table_size[kTableSize-1];
}

}

#endif    // HASH_TABLE_INL_H
