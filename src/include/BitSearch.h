//! @file
//! @brief BitBoardの検索機能/パターンマッチングを実装
//! @author Koichi NABETANI
//! @date 2017/03/10
#ifndef BIT_SEARCH_H
#define BIT_SEARCH_H

#include <cstdint>
#include <array>
#include <vector>

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
//! @pre strは32文字以内であること
const BitBoard GetStateBit(const std::string &str);

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
inline const BitBoard GetConsectiveStoneBit(const BitBoard stone_bit);

//! @brief [BnO1][WnO1]パターンを検索する
//! @param N パターン長(=n+1)
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @param pattern_bit_list 検索結果フラグ
//! @note pattern_bit_listにはOが最右のパターン、右から２つ目のパターン、‥、最左のパターンに対応する検索結果が格納される
//! @note 検索結果には合致したパターンの最小シフト量の位置に１を立てた値が入る
template<std::size_t N>
inline void GetStoneWithOneOpenBit(const BitBoard stone_bit, const BitBoard open_bit, std::array<BitBoard, N> * const pattern_bit_list);

//! @brief 右端ビットを求める
//! @param bit_board bit board値
//! @retval 右端ビット(例: 0b01011010 -> 0b00000010)
//! @pre bit_boardは0でないこと
inline BitBoard GetRightmostBit(const BitBoard bit_board);

//! @brief 右端のビット位置を求める(右端ビット指定版)
//! @param bit_board bit board値
//! @param rightmost_bit 右端ビット
//! @retval 右端のビット位置(0〜63)
//! @pre bit_boardは0でないこと
//! @see 一番右端の立っているビット位置を求める「ものすごい」コード(http://d.hatena.ne.jp/siokoshou/20090704)
inline size_t GetNumberOfTrailingZeros(const BitBoard bit_board, const BitBoard rightmost_bit);

//! @brief 右端のビット位置を求める(右端ビット指定無版)
//! @param bit_board bit board値
//! @retval 右端のビット位置(0〜63)
//! @pre bit_boardは0でないこと
inline size_t GetNumberOfTrailingZeros(const BitBoard bit_board);

//! @brief ビット位置のリストを取得する
//! @param bit_board bit board値
//! @param index_list ビット位置の格納先
inline void GetBitIndexList(BitBoard bit_board, std::vector<size_t> * const index_list);

}   // namespace realcore

#include "BitSearch-inl.h"

#endif    // BIT_SEARCH_H