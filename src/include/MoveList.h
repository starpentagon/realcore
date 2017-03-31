//! @file
//! @brief 指し手リストクラスの定義
//! @author Koichi NABETANI
//! @date 2017/02/21
#ifndef MOVE_LIST_H
#define MOVE_LIST_H

#include <vector>
#include <string>

namespace realcore{
class MoveList;
class MoveListTest;
enum MovePosition : std::uint8_t;
enum BoardSymmetry : std::uint8_t;

//! @brief [a-o][a-o]形式の文字列に対応する指し手リストを生成する
//! @param move_string [a-o][a-o]形式の文字列
//! @param move_list 指し手リストの格納先
//! @retval 文字列から指し手リストを生成成功
//! @pre move_listは空であること
//! @note 生成に失敗した場合はmove_listを初期化し空に戻す
bool GetMoveList(const std::string &move_string, MoveList * const move_list);

//! @brief 対称変換した指し手リストを生成する
//! @param move_list 変換対称の指し手リスト
//! @param symmetry 対称性
//! @param symmetric_move_list 対称変換した指し手リストの格納先
void GetSymmetricMoveList(const MoveList &move_list, const BoardSymmetry symmetry, MoveList * const symmetric_move_list);

//! @brief 指し手リストの管理クラス
//! @detail 以下の機能を提供する
//! - 指し手のリスト管理
//! - [a-o][a-o]形式の出力
class MoveList
{
  friend class MoveListTest;

public:
  MoveList();
  MoveList(const MoveList &move_list);
  MoveList(const MovePosition move);

  //! @brief [a-o][a-o]形式の文字列に対応する指し手リストを生成する
  MoveList(const std::string &move_str);

  //! @brief 黒番かどうかを返す
  //! @param true 黒番
  const bool IsBlackTurn() const;

  //! @brief リストの最終手を返す
  //! @note リストが空の時はkInvalidMoveを返す
  const MovePosition GetLastMove() const;

    //! @brief 指し手リストの長さを返す
  //! @retval 指し手リストの長さ
  const size_t size() const;

  //! @brief 指し手リストを空にする
  void clear();

  //! @brief 指し手リストが空かどうかを返す
  const bool empty() const;

  //! @brief [a-o][a-o]形式の文字列を返す
  //! @retval 指し手リストを[a-o][a-o]形式に変換した文字列
  //! @pre 指し手は有効な指し手であること
  std::string str() const;

  //! @brief 初期化時の領域を確保する
  //! @param initial_size 初期化時の指し手リスト長
  //! @note 事前に領域を確保することで領域の再確保を抑制しパフォーマンスを改善できる
  //! @note 領域サイズはCalcInitialReserveSize()で算出する(initial_list_sizeより少し大きい領域が確保される)
  void ReserveInitial(const size_t initial_list_size);

  //! @brief 代入演算子
  const MoveList& operator=(const MoveList &move_list);
  const MoveList& operator=(const MovePosition move);

  //! @brief 連結演算子
  const MoveList& operator+=(const MoveList &move_list);
  const MoveList& operator+=(const MovePosition move);

  //! @brief 比較演算子
  const bool operator==(const MoveList &move_list) const;
  const bool operator!=(const MoveList &move_list) const;
  
  //! @brief 添字演算子(右辺値)
  const MovePosition operator[](const size_t index) const;

  //! @brief 添字演算子(左辺値)
  MovePosition& operator[](const size_t index);

  //! @brief デクリメント演算子
  const MoveList& operator--();

  //! @breif 範囲の開始イテレータを返す
  std::vector<MovePosition>::const_iterator begin() const;

  //! @breif 範囲の終端イテレータを返す
  std::vector<MovePosition>::const_iterator end() const;

private:
  //! @brief 初期化時に確保する領域長さを算出する
  //! @param initial_size 初期化時の指し手リスト長
  //! @retval 確保する領域長
  //! @note 領域の再確保を抑制するため「初期化時のリスト長 + 16」(=L_0)より大きい領域を確保する
  //! @note MovePosition型は1バイトのため8バイト単位で確保するためL_0より大きい最小の8の倍数を返す
  const size_t CalcInitialReserveSize(const size_t initial_list_size) const;

  //! @brief 指し手リスト
  std::vector<MovePosition> move_list_;
};
}   // namespace realcore

#include "MoveList-inl.h"

#endif    // MOVE_LIST_H