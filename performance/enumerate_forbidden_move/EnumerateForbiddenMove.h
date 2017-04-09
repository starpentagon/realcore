#ifndef ENUMERATE_FORBIDDEN_MOVE_H
#define ENUMERATE_FORBIDDEN_MOVE_H

#include "MoveList.h"
#include "Board.h"

//! @brief 各空点が禁手かどうかをチェックする
//! @param board_move チェックする盤面の指し手リスト
//! @param forbidden_move 禁手の格納先
void CheckEachPoint(const realcore::MoveList &board_move, realcore::MoveList * const forbidden_move);

//! @brief 空点状態を使って列挙する
//! @param board チェックする盤面
//! @param forbidden_move 禁手の格納先
void EnumerateOpenState(const realcore::Board &board, realcore::MoveList * const forbidden_move);

//! @brief 空点状態を使って列挙する
//! @param board チェックする盤面
//! @param forbidden_move 禁手の格納先
void EnumerateOpenState(const realcore::BitBoard &bit_board, realcore::MoveList * const forbidden_move);

#endif
