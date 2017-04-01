#ifndef ENUMERATE_FORBIDDEN_MOVE_H
#define ENUMERATE_FORBIDDEN_MOVE_H

#include "MoveList.h"

//! @brief 各空点が禁手かどうかをチェックする
//! @param board_move チェックする盤面の指し手リスト
//! @param forbidden_move 禁手の格納先
void CheckEachPoint(const realcore::MoveList &board_move, realcore::MoveList * const forbidden_move);

#endif
