//! @file
//! @brief Open address法によるHashTableの基底クラス
//! @author Koichi NABETANI
//! @date 2017/04/24
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <cstdint>
#include <vector>

#include "Move.h"
#include "Lock.h"

namespace realcore
{

// Hash値
typedef std::uint64_t HashValue;

// 論理カウンタ
typedef std::uint16_t TableLogicCounter;

// 前方宣言
class MoveList;
class HashTableTest;

//! @brief Hash値を求める
const HashValue CalcHashValue(const MoveList &board_move_sequence);

//! @brief SymmetricHash値を求める
//! @note 対称変換で同一になる局面は同一のHash値になる
const HashValue CalcSymmetricHashValue(const MoveList &board_move_sequence);

//! @brief Hash値を求める(差分計算用)
const HashValue CalcHashValue(const bool is_black_turn, const MovePosition move, const HashValue current_value);

//! @brief Hash tableの管理クラス
template<class T>
class HashTable
{
  friend class HashTableTest;

public:
  //! @brief Hash tableを確保する
  //! @param table_mb_size HashTableのサイズ(MB)
  HashTable(const size_t table_space, const bool lock_control);

  ~HashTable();

  //! @brief 初期化を行う
  void Initialize();

  //! @brief Hash tableへのupsertを行う
  void Upsert(const HashValue hash_value, const T &element);

  //! @brief Hash tableの検索を行う
  const bool find(const HashValue hash_value, T * const element) const;

  //! @brief 競合が生じるかチェックする
  //! @param element 競合するデータを格納する
  const bool IsConflict(const HashValue hash_value, T * const element) const;

  //! @brief Hash tableの要素数を返す
  const size_t size() const;

  //! @brief Hash tableの確保したメモリ量(MB)を返す
  const double space() const;

private:
  //! @brief Hash値に対応するHash tableのindexを取得する
  const size_t GetTableIndex(const HashValue hash_value) const;
  
  //! @brief 論理カウンタによる論理的な初期化を行う
  void LogicalInitialize();

  //! @brief 全領域のクリアを行う
  void clear();

  //! @brief Hash tableの要素数を返す
  //! @note (sizeof(T) + sizeof(boost::mutex*)) * N が table_space(MB)以下になる素数を返す
  //! @note 要素数が1,531以下の時は1,531を返す
  //! @note 要素数が1,610,612,711以上の時は1,610,612,711を返す
  static constexpr size_t CalcHashTableSize(const size_t table_space);

  std::vector<T> hash_table_;   //!< Hash table

  bool lock_control_;           //!< Hash tableのlockフラグ
  std::vector<boost::mutex*> mutex_list_;   //!< Lock用mutexのリスト
  TableLogicCounter logic_counter_;   //!< 論理カウンタ(T要素の初期値が0のためHashTableでは1から開始する)
};

}   // namespace realcore

#include "HashTable-inl.h"

#endif    // HASH_TABLE_H
