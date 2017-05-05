//! @file
//! @brief 指し手パターン検索の定義
//! @author Koichi NABETANI
//! @date 2017/03/22

#ifndef MOVE_PATTERN_SEARCH_H
#define MOVE_PATTERN_SEARCH_H

#include "BitSearch.h"
#include "OpenState.h"

namespace realcore{

//! @brief 長連(BBBBBB)が生じているか判定する
//! @param black_bit 黒石フラグ
//! @retval true 長連が生じている
inline const bool IsOverline(const std::uint64_t black_bit);

//! @brief 空点状態の指し手パターンのパターン検索数を返す
//! @param pattern 空点状態の指し手パターン(長連点, 達四点, etc)
//! @retval パターン検索数
inline constexpr size_t GetOpenStatePatternNum(const OpenStatePattern pattern);

//! @brief 空点状態パターンを検索する
//! @param Pattern 検索するパターン
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @param pattern_search_bit_list 検索結果の格納先
template<OpenStatePattern Pattern>
inline void SearchOpenStatePattern(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, GetOpenStatePatternNum(Pattern)> * const pattern_search_bit_list);

//! @brief 長連点(B[B3O1]B)が生じているか判定する
//! @param stone_bit 黒石フラグ
//! @param open_bit 空点フラグ
//! @param pattern_search_bit_list B[B3O1]Bパターンの検索結果
inline void SearchNextOverline(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, kFourStonePattern> * const pattern_search_bit_list);

//! @brief 達四を検索する
//! @param P 手番
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @retval XOBBBBOX(黒番), OWWWWO(白番)パターンのうちBBBB, WWWWの右端位置に1を立てた値
template<PlayerTurn P>
inline const std::uint64_t SearchOpenFour(const std::uint64_t stone_bit, const std::uint64_t open_bit);

//! @brief 達四点を検索する
//! @param P 手番
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @param pattern_search_bit_list XO[B3O1]OX, O[W3O1]Oパターンの検索結果
template<PlayerTurn P>
inline void SearchNextOpenFour(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, kFourStonePattern> * const pattern_search_bit_list);

//! @brief 四を検索する
//! @param P 手番
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @param guard_move_bit 四の防手位置のフラグ
//! @retval X[B4O1]X(黒番), [W4O1](白番)パターンのうち[(B|W)4O1]の右端の位置に1を立てた値
template<PlayerTurn P>
inline const std::uint64_t SearchFour(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::uint64_t * const guard_move_bit);

template<PlayerTurn P>
inline const std::uint64_t SearchFour(const std::uint64_t stone_bit, const std::uint64_t open_bit);

//! @brief 四ノビ点を検索する
//! @param P 手番
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @param pattern_search_bit_list X[B3O2]X, [W3O2]パターンの検索結果
template<PlayerTurn P>
inline void SearchNextFour(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, kTwoOfFivePattern> * const pattern_search_bit_list);

//! @brief 見かけの三(Semi-three)を検索する
//! @param P 手番
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @param next_open_four_bit 達四になる位置のフラグ
//! @retval XO[B3O1]OX(黒番), O[W3O1]O(白番)パターンのうち[(B|W)3O1]の右端の位置に1を立てた値
template<PlayerTurn P>
inline const std::uint64_t SearchSemiThree(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::uint64_t * const next_open_four_bit);

//! @brief 見かけの三ノビ点を検索する
//! @param P 手番
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @param pattern_search_bit_list XO[B2O2]OX, O[W2O2]Oパターンのうち[(B|W)2O2]の右端の位置に1を立てた値
template<PlayerTurn P>
inline void SearchNextSemiThree(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, kTwoOfFourPattern> * const pattern_search_bit_list);

//! @brief 剣先点を検索する
//! @param P 手番
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @param pattern_search_bit_list X[B2O3]X, [W2O3]パターンのうち[(B|W)2O3]の右端の位置に1を立てた値
template<PlayerTurn P>
inline void SearchNextPointOfSword(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, kThreeOfFivePattern> * const pattern_search_bit_list);

}   // namespace realcore

#include "MovePatternSearch-inl.h"

#endif    // MOVE_PATTERN_SEARCH_H
