//! @file
//! @brief 指し手パターン検索の定義
//! @author Koichi NABETANI
//! @date 2017/03/22

#ifndef MOVE_PATTERN_SEARCH_H
#define MOVE_PATTERN_SEARCH_H

#include "BitSearch.h"

namespace realcore{

//! @brief 長連(BBBBBB)が生じているか判定する
//! @param black_bit 黒石フラグ
//! @retval true 長連が生じている
inline const bool IsOverline(const std::uint64_t black_bit);

//! @brief 達四を検索する
//! @param P 手番
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @retval XOBBBBOX(黒番), OWWWWO(白番)パターンのうち右端のOの位置に1を立てた値
template<PlayerTurn P>
inline const std::uint64_t SearchOpenFour(const std::uint64_t stone_bit, const std::uint64_t open_bit);

//! @brief 四を検索する
//! @param P 手番
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @retval X[B4O1]X(黒番), [W4O1](白番)パターンのうち[(B|W)4O1]の右端の位置に1を立てた値
template<PlayerTurn P>
inline const std::uint64_t SearchFour(const std::uint64_t stone_bit, const std::uint64_t open_bit);

//! @brief 見かけの三(Semi-three)を検索する
//! @param P 手番
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @param next_open_four_bit 達四になる位置のフラグ
//! @retval XO[B3O1]OX(黒番), O[W3O1]O(白番)パターンのうち右端のOの位置に1を立てた値
template<PlayerTurn P>
inline const std::uint64_t SearchSemiThree(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::uint64_t * const next_open_four_bit);

}   // namespace realcore

#include "MovePatternSearch-inl.h"

#endif    // MOVE_PATTERN_SEARCH_H
