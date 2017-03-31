//! @file
//! @brief RealCore全体で用いる定数等を定義する
//! @author Koichi NABETANI
//! @date 2017/02/20

#ifndef REAL_CORE_H
#define REAL_CORE_H

#include <cstddef>
#include <cstdint>
#include <array>

namespace realcore
{
//! @brief 座標を表す型
typedef std::int8_t Cordinate;

//! @brief 盤のサイズ
constexpr size_t kBoardLineNum = 15;

//! @brief 盤面方向の定義
enum BoardDirection : std::uint8_t
{
  kLateralDirection,
  kVerticalDirection,
  kLeftDiagonalDirection,
  kRightDiagonalDirection
};

//! @brief 盤面の方向数
constexpr size_t kBoardDirectionNum = 4;

//! @brief 盤面方向のリストを返す
//! @retval 盤面方向のリスト
const std::array<BoardDirection, kBoardDirectionNum>& GetBoardDirection();

//! @brief 盤面上の対称性
enum BoardSymmetry : std::uint8_t
{
  kIdenticalSymmetry,     //!< 恒等変換: (x, y) => (x, y)
  kHorizontalSymmetry,    //!< 左右対称: (x, y) => (-x, y)
  kVerticalSymmetry,      //!< 上下対称: (x, y) => (x, -y)
  kCentricSymmetry,       //!< 天元対称: (x, y) => (-x, -y)
  kDiagonalSymmetry1,     //!< 対角対称: (x, y) => (y, x)
  kDiagonalSymmetry2,     //!< 対角対称: (x, y) => (-y, x)
  kDiagonalSymmetry3,     //!< 対角対称: (x, y) => (y, -x)
  kDiagonalSymmetry4,     //!< 対角対称: (x, y) => (-y, -x)
};

//! 対称性の数
constexpr size_t kBoardSymmetryNum = 8;

//! @brief 対称性のリストを返す
const std::array<BoardSymmetry,kBoardSymmetryNum>& GetBoardSymmetry();

//! @brief 手番
enum PlayerTurn : std::uint8_t
{
  kBlackTurn,
  kWhiteTurn
};

//! @brief 盤面上の点の状態定義
enum PositionState : std::uint64_t
{
  kOverBoard,     //!< 盤外(0b00)
  kBlackStone,    //!< 黒石(0b01)
  kWhiteStone,    //!< 白石(0b10)
  kOpenPosition   //!< 空点(0b11)
};

//! 手番に対応する石を返す
constexpr PositionState GetPlayerStone(const PlayerTurn player);

//! @brief 盤面位置
//! @see doc/01_data_definition/data_definition.pptx and board_definition.xlsx
typedef std::uint64_t BoardPosition;

}   // namespace realcore

#include "RealCore-inl.h"

#endif
