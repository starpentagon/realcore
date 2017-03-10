//! @file
//! @brief BitBoardの検索機能/パターンマッチングを実装
//! @author Koichi NABETANI
//! @date 2017/03/10
#ifndef BIT_SEARCH_H
#define BIT_SEARCH_H

#include <cstdint>
#include <array>

namespace realcore{

//! @brief 盤面の状態
typedef std::uint64_t BitBoard;

//! @brief 盤面上の点の状態定義
enum PositionState : BitBoard
{
  kOverBoard,     //!< 盤外(0b00)
  kBlackStone,    //!< 黒石(0b01)
  kWhiteStone,    //!< 白石(0b10)
  kOpenPosition   //!< 空点(0b11)
};

//! @brief 状態文字列([B|W|O|X]*)に対応するbit board値を返す
//! @param str 状態文字列([B|W|O|X]*)
//! @retval 対応するbit board値
//! @pre strは３２文字以内であること
inline BitBoard GetStateBit(const std::string &str);

//! @brief 黒石フラグを返す
//! @param bit_board bit board値
//! @retval 黒石の位置に1を立てた値
inline constexpr BitBoard GetBlackStoneBit(const BitBoard bit_board);

//! @brief 白石フラグを返す
//! @param bit_board bit board値
//! @retval 白石の位置に1を立てた値
inline constexpr BitBoard GetWhiteStoneBit(const BitBoard bit_board);

//! @brief 空点フラグを返す
//! @param bit_board bit board値
//! @retval 空点の位置に1を立てた値
inline constexpr BitBoard GetOpenPositionBit(const BitBoard bit_board);

//! @brief 連続する同一N個の石フラグを返す
//! @param N 連続する石数
//! @param stone_bit 黒石 or 白石フラグ
//! @retval N個が連続するパターンのうち最小シフト量の位置に1を立てた値
template<std::size_t N>
inline BitBoard GetConsectiveStoneBit(const BitBoard stone_bit);

//! @brief [BnO1][WnO1]パターンを検索する
//! @param N パターン長(=n+1)
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @param pattern_bit_list 検索結果フラグ
//! @note pattern_bit_listにはOが最右のパターン、右から２つ目のパターン、‥、最左のパターンに対応する検索結果が格納される
//! @note 検索結果には合致したパターンの最小シフト量の位置に１を立てた値が入る
template<std::size_t N>
inline void GetStoneWithOneOpenBit(const BitBoard stone_bit, const BitBoard open_bit, std::array<BitBoard, N> * const pattern_bit_list);

}   // namespace realcore

#include "BitSearch-inl.h"

#endif    // BIT_SEARCH_H