//! @file
//! @brief 指し手定義、指し手集合オブジェクトの定義
//! @author Koichi NABETANI
//! @date 2017/02/20

#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include <array>

#include "RealCore.h"

namespace realcore
{

//! @brief Move位置
//! @see doc/01_data_definition/data_definition.pptx and move_definition.xlsx
enum MovePosition : std::uint8_t
{
  #include "def/MoveDefinition.h"
};

//! @brief Move位置の数
constexpr size_t kMoveNum = 256;

//! @brief 盤内の指し手の数
constexpr size_t kInBoardMoveNum = kBoardLineNum * kBoardLineNum;

//! @brief 有効な指し手(盤内 or Pass)の数
constexpr size_t kValidMoveNum = (kInBoardMoveNum + 1);

//! @brief すべての指し手リストを返す
//! @retval MovePosition型として定義されているすべての指し手の配列を返す
const std::array<MovePosition, kMoveNum>& GetAllMove();

//! @brief 有効な指し手(盤内 or Pass)リストを返す
const std::array<MovePosition, kValidMoveNum>& GetAllValidMove();

//! @brief 盤内の指し手リストを返す
const std::array<MovePosition, kInBoardMoveNum>& GetAllInBoardMove();

//! @brief 指し手が無効かどうか判定する
//! @param move 指し手位置
//! @retval true 指し手が無効
const bool IsInvalidMove(const MovePosition move);

//! @brief 指し手がPassかどうかを判定する
//! @param move 指し手位置
//! @retval true 指し手がPass
const bool IsNullMove(const MovePosition move);

//! @brief 点(x, y)が盤内かを判定する関数
//! @param x x座標
//! @param y y座標
//! @retval true 点(x, y)が盤内
//! @pre x, yは[0, 15]の範囲であること
constexpr inline bool IsInBoard(const Cordinate x, const Cordinate y);

//! @brief 指し手が盤内かどうかを判定する
//! @param move 指し手位置
//! @retval true 指し手が盤内
const bool IsInBoardMove(const MovePosition move);

//! @brief 指し手が有効(盤内 or Pass)指し手が無効かどうか判定する
//! @param move 指し手位置
//! @retval true 指し手が有効(盤内 or Pass)
const bool IsValidMove(const MovePosition move);

//! @brief 指し手の(x, y)座標を求める
//! @param move 指し手位置
//! @param x x座標の格納先
//! @param y y座標の格納先
//! @note x, yともに[0, 15]であることを保証する
void GetMoveCordinate(const MovePosition move, Cordinate * const x, Cordinate * const y);

//! @brief (x, y)座標からMovePositionを求める
//! @param x x座標の格納先
//! @param y y座標の格納先
//! @retval 対応するMovePosition
//! @pre (x, y)は盤内にあること
const MovePosition GetMove(const Cordinate x, const Cordinate y);

//! @brief 対称変換した指し手位置を求める
//! @param move 指し手位置
//! @retval 対称変換した位置
const MovePosition GetSymmetricMove(const MovePosition move, const BoardSymmetry symmetry);

}   // namespace　realcore

#include "Move-inl.h"

#endif // MOVE_H
