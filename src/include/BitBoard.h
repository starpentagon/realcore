//! @file
//! @brief BitBoard関連の定義
//! @author Koichi NABETANI
//! @date 2017/03/31

#ifndef BIT_BOARD_H
#define BIT_BOARD_H

#include <cstdint>
#include <array>
#include <bitset>

#include "BitSearch.h"
#include "BoardOpenState.h"

namespace realcore
{
// 指し手のビットを管理するbitset
typedef std::bitset<kMoveNum> MoveBitSet;

//! @brief Bitboard(=StateBit配列)の要素数
constexpr size_t kBitBoardElementNum = 32;

//! @brief Bitboard型
typedef std::array<StateBit, kBitBoardElementNum> Bitboard;

// 前方宣言
enum MovePosition : std::uint8_t;
class BoardOpenState;
class BitBoard;
class BitBoardTest;
class MoveList;

//! MoveBitSetから指し手リストを取得する
void GetMoveList(const MoveBitSet &move_bit_set, MoveList *move_list);

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
  friend class BitBoardTest;
  friend bool IsEqual(const BitBoard &bit_board_1, const BitBoard &bit_board_2);
  friend void Copy(const BitBoard &bit_board_from, BitBoard * const bit_board_to);

public:
  BitBoard();
  BitBoard(const BitBoard &bit_board);
  BitBoard(const MoveList &move_list);

  //! @brief 代入演算子
  const BitBoard& operator=(const BitBoard &bit_board);

  //! @brief 比較演算子
  const bool operator==(const BitBoard &bit_board) const;
  const bool operator!=(const BitBoard &bit_board) const;

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

  //! @brief 指し手が達四かチェックする
  //! @param move 指し手位置
  //! @retval true 指し手が達四
  //! @pre moveは着手前であること
  template<PlayerTurn P>
  const bool IsOpenFourMove(const MovePosition move) const;

  //! @brief 指し手が四かチェックする(着手前版)
  //! @param move 指し手位置
  //! @param guard_move 防手位置
  //! @retval true 指し手が四
  //! @pre moveは着手前であること
  template<PlayerTurn P>
  const bool IsFourMove(const MovePosition move, MovePosition * const guard_move) const;

  //! @brief 指し手が四かチェックする(着手後版)
  //! @param move 指し手位置
  //! @param guard_move 防手位置
  //! @retval true 指し手が四
  //! @pre moveは着手後であること
  template<PlayerTurn P>
  const bool IsFourMoveOnBoard(const MovePosition move, MovePosition * const guard_move) const;

  //! @brief 指し手が四々かチェックする
  //! @param move 指し手位置
  //! @retval true 指し手が四々
  //! @pre moveは着手前であること
  template<PlayerTurn P>
  const bool IsDoubleFourMove(const MovePosition move) const;

  //! @brief 指し手が禁手かチェックする
  //! @param move 指し手位置
  //! @retval true 指し手が禁手
  //! @pre moveは着手前であること
  template<PlayerTurn P>
  const bool IsForbiddenMove(const MovePosition move) const;

  //! @brief 禁点を列挙する
  //! @param 禁点の格納先
  void EnumerateForbiddenMoves(const BoardOpenState &board_open_state, MoveBitSet * const forbidden_move_set) const;
  void EnumerateForbiddenMoves(MoveBitSet * const forbidden_move_set) const;

private:
  //! @brief 盤面の空点状態を取得する
  //! @param board_open_state 空点状態の格納先
  void GetBoardOpenState(const UpdateOpenStateFlag &update_flag, BoardOpenState * const board_open_state) const;

  //! 指し手パターンの空点状態を取得する
  template<OpenStatePattern Pattern>
  void GetOpenState(const size_t index, const std::uint64_t combined_stone_bit, const std::uint64_t combined_open_bit, BoardOpenState * const board_open_state) const;
  
  //! @brief 空点状態を追加する
  //! @param pattern_search_index 検索パターンのindex
  //! @param pattern_position 検索パターンのマッチ位置
  //! @param board_open_state 空点状態の格納先
  template<OpenStatePattern Pattern>
  void AddOpenState(const size_t pattern_search_index, const BoardPosition pattern_position, BoardOpenState * const board_open_state) const;

  Bitboard bit_board_;    //!< 盤面状態を保持するBit board
};    // class BitBoard

}   // namespace realcore

#include "BitBoard-inl.h"

#endif    // BIT_BOARD_H

