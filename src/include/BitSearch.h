//! @file
//! @brief StateBitの検索機能/パターンマッチングを実装
//! @author Koichi NABETANI
//! @date 2017/03/10
#ifndef BIT_SEARCH_H
#define BIT_SEARCH_H

#include <cstdint>
#include <array>
#include <vector>

#include "RealCore.h"

namespace realcore{

//! @brief 盤面の状態
typedef std::uint64_t StateBit;

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
inline constexpr StateBit RightShift(const StateBit state_bit);

//! @brief 状態を左シフトする
//! @param N 左シフトする状態数
//! @param state_bit StateBit
//! @retval シフト後のStateBit
template<size_t N>
inline constexpr StateBit LeftShift(const StateBit state_bit);

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

//! @brief プレイヤーの石フラグを返す
//! @param state_bit State Bit
//! @retval プレイヤーの石の位置に1を立てた値
template<PlayerTurn P>
constexpr std::uint64_t GetPlayerStoneBit(const StateBit state_bit);

static constexpr size_t kSixStonePattern = 6;    //<! 6個の石のパターン
static constexpr size_t kFiveStonePattern = 5;   //<! 5個の石のパターン
static constexpr size_t kFourStonePattern = 4;   //<! 4個の石のパターン

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

//! @brief [BnO1][WnO1]パターンのOの位置を返す
//! @param index 検索結果のindex
//! @param pattern_bit 検索結果フラグ
inline const std::uint64_t GetOpenBitInPattern(const size_t index, const std::uint64_t pattern_bit);

//! @brief [BnOm][WnOm]のパターン位置(BoardPosition)と空点indexから空点のBoardPositionを返す
inline const BoardPosition GetOpenBoardPosition(const BoardPosition pattern_position, const size_t open_index);

static const size_t kTwoOfFourPattern = 6;   // = 4C2
static const size_t kTwoOfFivePattern = 10;  // = 10C2

//! @brief [BnO2][WnO2]パターンを検索する
//! @param N パターン長(=n+2C2=(n+2)(n+1)/2)
//! @param stone_bit 黒石 or 白石フラグ
//! @param open_bit 空点フラグ
//! @param pattern_bit_list 検索結果フラグ
//! @note pattern_bit_listにはOの位置が(0, 1)(0, 2),...,(3, 4)に対応する検索結果が格納される
//! @note 検索結果には合致したパターンの最小シフト量の位置に１を立てた値が入る
template<size_t N>
inline void GetStoneWithTwoOpenBit(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, N> * const pattern_bit_list);

//! @brief [BnO2][WnO2]パターンのOの位置の小さい方を返す
inline const size_t GetLessIndexOfTwo(const size_t index);

//! @brief [BnO2][WnO2]パターンのOの位置の大きい方を返す
inline const size_t GetGreaterIndexOfTwo(const size_t index);

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