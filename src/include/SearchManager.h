//! @file
//! @brief 探索情報を管理する
//! @author Koichi NABETANI
//! @date 2017/04/19

#ifndef SEARCH_MANAGER_H
#define SEARCH_MANAGER_H

#include <cstdint>
#include <chrono>

namespace realcore{

typedef std::uint64_t SearchCounter;

static constexpr bool kCatchInterruptException = true;    //!< interrupt exceptionをcatchする
static constexpr bool kNoInterruptionPoint = false;       //!< interrupt exceptionをcatchしない

// 前方宣言
class SearchManagerTest;

class SearchManager
{
  friend class SearchManagerTest;

public:
  SearchManager(const bool catch_interrupt_exception);

  //! @brief 探索ノードを増やす
  void AddNode();

  //! @brief 探索ノードを取得する
  SearchCounter GetNode() const;

  //! @brief 探索ノード上限を設定する
  void SetNodeLimit(const SearchCounter node_limit);

  //! @brief 探索時間上限(ms)を設定する
  void SetSearchTimeLimit(const SearchCounter elapsed_time_limit);

  //! @brief 探索時間の計測を開始する
  void SearchStart();

  //! @brief 探索時間(ms)を取得する
  SearchCounter GetSearchTime() const;

  //! @brief 探索終了条件が成立しているか返す
  const bool IsTerminate();

private:
  SearchCounter node_;        //!< 探索ノード数
  SearchCounter node_limit_;  //!< 探索ノード数の上限(0: 無制限)

  std::chrono::system_clock::time_point search_start_time_;   //!< 探索開始時間
  SearchCounter elapsed_time_limit_;  //!< 探索時間の上限

  bool catch_interrupt_exception_;    //!< Interrupt Exceptionをcatchするかどうかのフラグ
  bool interruption_;   //!< interruption exceptionが生じたかどうかのフラグ
};

}   // namespace realcore

#include "SearchManager-inl.h"

#endif    // SEARCH_MANAGER_H
