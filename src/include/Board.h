//! @file
//! @brief 盤面の管理クラス
//! @author Koichi NABETANI
//! @date 2017/02/22

#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <array>

#include "RealCore.h"
#include "BitSearch.h"

namespace realcore
{
//! @brief 点(x, y)が盤内かを判定する関数
//! @param x x座標
//! @param y y座標
//! @retval true 点(x, y)が盤内
//! @pre x, yは[0, 15]の範囲であること
constexpr inline bool IsInBoard(const Cordinate x, const Cordinate y);

//! @brief 盤面位置
//! @see doc/01_data_definition/data_definition.pptx and board_definition.xlsx
typedef std::uint64_t BoardPosition;

//! @brief Bitboard(=StateBit配列)の要素数
constexpr size_t kBitBoardElementNum = 32;

//! @brief Bitboard型
typedef std::array<StateBit, kBitBoardElementNum> Bitboard;

// 前方宣言
enum MovePosition : std::uint8_t;
class BoardTest;
class MoveList;

//! @brief 盤面管理クラス
class Board
{
  friend class BoardTest;

public:
  Board();
  Board(const Board &board);
  Board(const MoveList &move_list);

  //! @brief 盤面状態を取得する(MovePosition版)
  //! @param move 指し手位置
  //! @retval 指定位置の盤面状態
  //! @note moveが盤内以外の場合、kOverBoardを返す
  const PositionState GetState(const MovePosition move) const;

  //! @brief 盤面状態を設定する(template版)
  //! @param State 設定する盤面状態
  //! @param move 指し手位置
  template<PositionState State>
  void SetState(const MovePosition move);
  
  //! @brief 盤面状態を設定する(non-template版)
  void SetState(const MovePosition move, const PositionState state);

  //! @brief 指定位置を中心としたN路の直線近傍の盤面状態(各方向2N+1個の状態)を取得する
  //! @param N 直線近傍の長さ
  //! @param move 指し手位置
  //! @param line_neighborhood_list 直線近傍の盤面状態の格納先
  //! @pre N in [1, 7]
  //! @note 指定位置を14-15bit目に揃えたビット（(14 - 2 * N)bit目〜(15 + 2 * N)bit目）を設定して返す
  //! @note moveが盤内でなければすべて盤外の状態を設定する
  template<size_t N>
  void GetLineNeighborhoodStateBit(const MovePosition move, std::array<StateBit, kBoardDirectionNum> * const line_neighborhood_list) const;

  //! @brief 盤面状態を文字列出力する
  //! @retval 盤面をテキスト表現した文字列
  const std::string str() const;

protected:
  //! @brief 盤面状態を取得する(BoardPosition版)
  //! @param board_position 盤面位置
  //! @retval 指定位置の盤面状態
  const PositionState GetState(const BoardPosition board_position) const;

  //! @brief BoardPositionに対応するBitBoard配列のindexを取得する
  //! @param board_position BoardPosition
  //! @retval BitBoard配列のindex
  const size_t GetBitBoardIndex(const BoardPosition board_position) const;

  //! @brief BoardPositionに対応するシフト量を取得する
  const size_t GetBitBoardShift(const BoardPosition board_position) const;

  //! @brief (x, y)座標から各方向のBitBoard配列のindexを取得する
  //! @param x x座標
  //! @param y y座標
  //! @param index_list BitBoard配列のindexの格納先
  //! @pre (x, y)は盤内であること
  void GetBitBoardIndexList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> * const index_list) const;

  //! @brief (x, y)座標から各方向のBitBoard配列のshift量を取得する
  //! @param x x座標
  //! @param y y座標
  //! @param index_list BitBoard配列のshift量の格納先
  //! @pre (x, y)は盤内であること
  void GetBitBoardShiftList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> * const shift_list) const;

  //! @brief 盤面状態を保持する配列
  Bitboard bit_board_;
};

}   // namespace realcore

#include "Board-inl.h"

#endif    // BOARD_H
