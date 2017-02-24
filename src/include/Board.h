//! @file
//! @brief 盤面の管理クラス
//! @author Koichi NABETANI
//! @date 2017/02/22

#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <array>

#include "RealCore.h"

namespace realcore
{
//! @brief 点(x, y)が盤内かを判定する関数
//! @param x x座標
//! @param y y座標
//! @retval true 点(x, y)が盤内
//! @pre x, yは[0, 15]の範囲であること
constexpr bool IsInBoard(const Cordinate x, const Cordinate y);

//! @brief 盤面の状態
typedef std::uint64_t BitBoard;

//! @brief 盤面位置
//! @see doc/01_data_definition/data_definition.pptx and board_definition.xlsx
typedef std::uint64_t BoardPosition;

//! @brief 盤面上の点の状態定義
enum PositionState : BitBoard
{
  kOverBoard,     //!< 盤外(0b00)
  kBlackStone,    //!< 黒石(0b01)
  kWhiteStone,    //!< 白石(0b10)
  kOpenPosition   //!< 空点(0b11)
};

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
  void GetBitBoardIndexList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> *index_list) const;

  //! @brief (x, y)座標から各方向のBitBoard配列のshift量を取得する
  //! @param x x座標
  //! @param y y座標
  //! @param index_list BitBoard配列のshift量の格納先
  //! @pre (x, y)は盤内であること
  void GetBitBoardShiftList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> *shift_list) const;

  //! @brief BitBoard配列の要素数
  static constexpr size_t kBitBoardNum = 32;

  //! @brief 盤面状態を保持する配列
  std::array<BitBoard, kBitBoardNum> bit_board_;
};

}   // namespace realcore

#include "Board-inl.h"

#endif    // BOARD_H
