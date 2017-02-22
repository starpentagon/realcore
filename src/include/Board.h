//! @file
//! @brief 盤面の管理クラス
//! @author Koichi NABETANI
//! @date 2017/02/22

#ifndef BOARD_H
#define BOARD_H

#include "RealCore.h"

namespace realcore
{
//! @brief 点(x, y)が盤内かを判定する関数
//! @param x x座標
//! @param y y座標
//! @retval true 点(x, y)が盤内
constexpr bool IsInBoard(const Cordinate x, const Cordinate y);

}   // namespace realcore

#include "Board-inl.h"

#endif    // BOARD_H
