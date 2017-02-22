//! @file
//! @brief RealCore全体で用いる定数等を定義する
//! @author Koichi NABETANI
//! @date 2017/02/20

#ifndef REAL_CORE_H
#define REAL_CORE_H

#include <cstddef>
#include <cstdint>

namespace realcore
{
//! @brief 座標を表す型
typedef std::int8_t Cordinate;

//! @brief 盤のサイズ
constexpr size_t kBoardLineNum = 15;

}   // namespace realcore

#endif
