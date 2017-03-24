//! @file
//! @brief StateBitの検索機能/パターンマッチングを実装
//! @author Koichi NABETANI
//! @date 2017/03/10
#ifndef BIT_SEARCH_H
#define BIT_SEARCH_H

#include <cstdint>
#include <array>
#include <vector>

namespace realcore{

//! @brief 盤面の状態
typedef std::uint64_t StateBit;

//! @brief 盤面上の点の状態定義
enum PositionState : StateBit
{
  kOverBoard,     //!< 盤外(0b00)
  kBlackStone,    //!< 黒石(0b01)
  kWhiteStone,    //!< 白石(0b10)
  kOpenPosition   //!< 空点(0b11)
};

//! @brief 禁手チェックの状態定義
enum ForbiddenCheckState : std::uint8_t
{
  kForbiddenMove,         //! 禁手
  kPossibleForbiddenMove, //! 見かけの三々があり禁手の可能性あり
  kNonForbiddenMove       //! 否禁
};

//! @brief 状態を右シフトする
//! @param N 右シフトする状態数
//! @param state_bit StateBit
//! @retval シフト後のStateBit
template<size_t N>
inline const StateBit RightShift(const StateBit state_bit);

//! @brief 状態を左シフトする
//! @param N 左シフトする状態数
//! @param state_bit StateBit
//! @retval シフト後のStateBit
template<size_t N>
inline const StateBit LeftShift(const StateBit state_bit);

//! @brief 状態文字列([B|W|O|X]*)に対応するStateBitを返す
//! @param str 状態文字列([B|W|O|X| ]*)
//! @retval 対応するstate_bit
//! @pre strは32文字以内であること
//! @note スペースは無視する
const StateBit GetStateBit(const std::string &str);

//! @brief StateBitに対応する状態文字列([B|W|O|X]*)を返す
//! @param state_bit StateBit
//! @retval 状態文字列([B|W|O|X]*)
//! @note 8文字ごとにスペースを入れた32+3=35文字の文字列を返す
const std::string GetStateBitString(StateBit state_bit);

//! @brief index間の差分値を算出する
//! @param index_from 起点となるindex値
//! @param index_to 差分を求めるindex値
//! @retval index間の差分値
inline const int GetIndexDifference(const size_t index_from, const size_t index_to);

//! @brief 黒石フラグを返す
//! @param state_bit State Bit
//! @retval 黒石の位置に1を立てた値
inline constexpr std::uint64_t GetBlackStoneBit(const StateBit state_bit);

//! @brief 白石フラグを返す
//! @param state_bit State Bit
//! @retval 白石の位置に1を立てた値
inline constexpr std::uint64_t GetWhiteStoneBit(const StateBit state_bit);

//! @brief 空点フラグを返す
//! @param state_bit State Bit
//! @retval 空点の位置に1を立てた値
inline constexpr std::uint64_t GetOpenPositionBit(const StateBit state_bit);

//! @brief 連続する同一N個の石フラグを返す
//! @param N 連続する石数
//! @param stone_bit 黒石 or 白石フラグ
//! @retval N個が連続するパターンのうち最小シフト量の位置に1を立てた値
template<size_t N>
inline const std::uint64_t GetConsectiveStoneBit(const std::uint64_t stone_bit);

//! @brief [BnO1][WnO1]パターンを検索する
//! @param N パターン長(=n+1)
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @param pattern_bit_list 検索結果フラグ
//! @note pattern_bit_listにはOが最右のパターン、右から２つ目のパターン、‥、最左のパターンに対応する検索結果が格納される
//! @note 検索結果には合致したパターンの最小シフト量の位置に１を立てた値が入る
template<size_t N>
inline void GetStoneWithOneOpenBit(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, N> * const pattern_bit_list);

//! @brief ビットの数が1つだけ立っているかをチェックする
//! @param bit ビット数を求めるbit(i=1,2)
//! @retval true ビットの数が1
//! @pre bitは0以外
inline const bool IsSingleBit(const std::uint64_t bit);

//! @brief ビットの数が2以上かどうかをチェックする(64bit変数 * 2版)
//! @param bit_i チェック対象のbit(i=1,2)
//! @retval true ビットの数が2以上
inline const bool IsMultipleBit(const std::uint64_t bit_1, const std::uint64_t bit_2);

//! @brief 右端ビットを求める
//! @param bit 右端ビットを求めるbit
//! @retval 右端ビット(例: 0b01011010 -> 0b00000010)
//! @pre state_bitは0でないこと
inline std::uint64_t GetRightmostBit(const std::uint64_t bit);

//! @brief 右端のビット位置を求める(右端ビット指定版)
//! @param bit 右端ビット位置を求めるbit
//! @param rightmost_bit 右端ビット
//! @retval 右端のビット位置(0〜63)
//! @pre state_bitは0でないこと
//! @see 一番右端の立っているビット位置を求める「ものすごい」コード(http://d.hatena.ne.jp/siokoshou/20090704)
inline size_t GetNumberOfTrailingZeros(const std::uint64_t bit, const std::uint64_t rightmost_bit);

//! @brief 右端のビット位置を求める(右端ビット指定無版)
//! @param bit 右端ビット位置を求めるbit
//! @retval 右端のビット位置(0〜63)
//! @pre state_bitは0でないこと
inline size_t GetNumberOfTrailingZeros(const std::uint64_t bit);

//! @brief ビット位置のリストを取得する
//! @param bit ビット位置リストを求めるbit
//! @param index_list ビット位置の格納先
inline void GetBitIndexList(std::uint64_t bit, std::vector<size_t> * const index_list);

//! @brief N個連続でbitが立つフラグを取得する
//! @retval N個連続でbitが立った値
//! @pre N in [1, 63]であること
template<size_t N>
inline constexpr std::uint64_t GetConsectiveBit();

}   // namespace realcore

#include "BitSearch-inl.h"

#endif    // BIT_SEARCH_H