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
enum BoardDirection
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

//! @brief 手番
enum PlayerTurn
{
  kBlackTurn,
  kWhiteTurn
};

}   // namespace realcore

#include "RealCore-inl.h"

#endif
