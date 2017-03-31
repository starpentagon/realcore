//! @file
//! @brief BitBoard関連の定義
//! @author Koichi NABETANI
//! @date 2017/03/31

#ifndef BIT_BOARD_H
#define BIT_BOARD_H

#include <cstdint>
#include <array>

#include "BitSearch.h"

namespace realcore
{
//! @brief 盤面位置
//! @see doc/01_data_definition/data_definition.pptx and board_definition.xlsx
typedef std::uint64_t BoardPosition;

//! @brief Bitboard(=StateBit配列)の要素数
constexpr size_t kBitBoardElementNum = 32;

//! @brief Bitboard型
typedef std::array<StateBit, kBitBoardElementNum> Bitboard;

// 前方宣言
enum MovePosition : std::uint8_t;
class BitBoard;
class BitBoardTest;
class MoveList;
class Board;

//! @brief 2つのBitBoardを比較する
//! @param bit_board_1, 2: 比較対象
//! @retval true 2つのBoardが同一の内容を保持
bool IsEqual(const BitBoard &bit_board_1, const BitBoard &bit_board_2);

//! @brief コピーを作成する
//! @param bit_board_from コピー元
//! @param bit_board_to コピー先
void Copy(const BitBoard &bit_board_from, BitBoard * const bit_board_to);

class BitBoard
{
  friend class BoardTest;
  friend bool IsEqual(const Board &board_1, const Board &board_2);
  friend void Copy(const Board &board_from, Board * const board_to);

public:
  BitBoard();
  BitBoard(const Board &board);
  BitBoard(const MoveList &move_list);

  //! @brief 代入演算子
  const BitBoard& operator=(const Board &board);

  //! @brief 比較演算子
  const bool operator==(const Board &board) const;
  const bool operator!=(const Board &board) const;

  //! @brief 盤面状態を取得する(MovePosition版)
  //! @param bit_board BitBoard
  //! @param move 指し手位置
  //! @retval 指定位置の盤面状態
  //! @note moveが盤内以外の場合、kOverBoardを返す
  const PositionState GetState(const MovePosition move) const;

  //! @brief 盤面状態を取得する(BoardPosition版)
  //! @param bit_board BitBoard
  //! @param board_position 盤面位置
  //! @retval 指定位置の盤面状態
  const PositionState GetState(const BoardPosition board_position) const;

  //! @brief 盤面状態を設定する(template版)
  //! @param State 設定する盤面状態
  //! @param bit_board BitBoard
  //! @param move 指し手位置
  template<PositionState State>
  void SetState(const MovePosition move);

  //! @brief 盤面状態を設定する(non-template版)
  void SetState(const MovePosition move, const PositionState state);

  //! @brief 指定位置を中心としたN路の直線近傍の盤面状態(各方向2N+1個の状態)を取得する
  //! @param N 直線近傍の長さ
  //! @param move 指し手位置
  //! @param line_neighborhood_list 直線近傍の盤面状態の格納先
  //! @pre N in [1, 7]
  //! @note 指定位置を14-15bit目に揃えたビット（(14 - 2 * N)bit目〜(15 + 2 * N)bit目）を設定して返す
  //! @note moveが盤内でなければすべて盤外の状態を設定する
  template<size_t N>
  void GetLineNeighborhoodStateBit(const MovePosition move, std::array<StateBit, kBoardDirectionNum> * const line_neighborhood_list) const;

  //! @brief 盤面状態を文字列出力する
  //! @retval 盤面をテキスト表現した文字列
  const std::string str() const;

  //! @brief 指し手が禁手かチェックする
  //! @param move 指し手位置
  //! @retval true 指し手が禁手
  //! @pre moveは着手前であること
  template<PlayerTurn P>
  const bool IsForbiddenMove(const MovePosition move) const;

private:
  Bitboard bit_board_;
};

// --- (x, y)座標 => index/shift ---
//! @brief BoardPositionに対応するBitBoard配列のindexを取得する
//! @param board_position BoardPosition
//! @retval BitBoard配列のindex
inline const size_t GetBitBoardIndex(const BoardPosition board_position);

//! @brief BoardPositionに対応するシフト量を取得する
inline const size_t GetBitBoardShift(const BoardPosition board_position);

//! @brief (x, y)座標から各方向のBitBoard配列のindexを取得する
//! @param x x座標
//! @param y y座標
//! @param index_list BitBoard配列のindexの格納先
//! @pre (x, y)は盤内であること
inline void GetBitBoardIndexList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> * const index_list);

//! @brief (x, y)座標から各方向のBitBoard配列のshift量を取得する
//! @param x x座標
//! @param y y座標
//! @param index_list BitBoard配列のshift量の格納先
//! @pre (x, y)は盤内であること
inline void GetBitBoardShiftList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> * const shift_list);

// --- (index, shift) => BoardPosition ---
//! @brief BitBoard配列のindexとshift量からBoardPositionを算出する関数
//! @param index BitBoard配列のindex
//! @param shift BitBoard配列のshift量
//! @retval 対応するBoardPosition
inline const BoardPosition GetBoardPosition(const size_t index, const size_t shift);

// --- BoardPosition => BoardDirection ---
//! @brief BoardPositionから対応する方向を返す関数
//! @param board_position BoardPosition
//! @retval BoardPositionに対応する方向
inline const BoardDirection GetBoardDirection(const BoardPosition board_position);

// --- BoardPosition => (x, y)/MovePosition ---
//! @brief BoardPositionから(x, y)座標を求める
//! @param board_position BoardPosition
//! @param x x座標
//! @param y y座標
//! @note x, yともに[0, 15]であることを保証する
inline const void GetBoardCordinate(const BoardPosition board_position, Cordinate * const x, Cordinate * const y);

//! @brief BoardPositionからMovePositionを求める
//! @param board_position BoardPosition
//! @retval BoardPositionに対応するMovePosition
inline const MovePosition GetBoardMove(const BoardPosition board_position);

}   // namespace realcore

#endif    // BIT_BOARD_H

