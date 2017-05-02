//! @file
//! @brief 変換関数群を定義する
//! @author Koichi NABETANI
//! @date 2017/04/09

#ifndef CONVERSION_H
#define CONVERSION_H

#include "RealCore.h"

namespace realcore
{

// 前方宣言
enum MovePosition : std::uint8_t;

// --- BoardPosition/(x, y)座標 => index/shift ---
//! @brief BoardPositionに対応するBitBoard配列のindexを取得する
//! @param board_position BoardPosition
//! @retval BitBoard配列のindex
inline const size_t GetBitBoardIndex(const BoardPosition board_position);

//! @brief (x, y)座標, BoardDirectionに対応するBitBoard配列のindexを取得する(template版)
//! @param kDirection 盤面方向
//! @param x x座標
//! @param y y座標
//! @retval BitBoard配列のindex
template<BoardDirection kDirection>
inline const size_t GetBitBoardIndex(const Cordinate x, const Cordinate y);

//! @brief (x, y)座標, BoardDirectionに対応するBitBoard配列のindexを取得する(non-template版)
inline const size_t GetBitBoardIndex(const Cordinate x, const Cordinate y, const BoardDirection direction);

//! @brief BoardPositionに対応するシフト量を取得する
inline const size_t GetBitBoardShift(const BoardPosition board_position);

//! @brief (x, y)座標, BoardDirectionに対応するBitBoard配列のシフト量を取得する(template版)
template<BoardDirection kDirection>
inline const size_t GetBitBoardShift(const Cordinate x, const Cordinate y);

//! @brief (x, y)座標, BoardDirectionに対応するBitBoard配列のシフト量を取得する(non-template版)
inline const size_t GetBitBoardShift(const Cordinate x, const Cordinate y, const BoardDirection direction);

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

// --- (index, shift)/(MovePosition, BoardDirection) => BoardPosition ---
//! @brief BitBoard配列のindexとshift量からBoardPositionを算出する関数
//! @param index BitBoard配列のindex
//! @param shift BitBoard配列のshift量
//! @retval 対応するBoardPosition
inline const BoardPosition GetBoardPosition(const size_t index, const size_t shift);

//! @brief MovePositionとBoardDirectionからBoardPositionを算出する関数
//! @param move 指し手位置
//! @param direction 盤面方向
//! @retval 対応するBoardPosition
inline const BoardPosition GetBoardPosition(const MovePosition move, const BoardDirection direction);

// --- BoardPosition => BoardDirection ---
//! @brief BoardPositionから対応する方向を返す関数
//! @param board_position BoardPosition
//! @retval BoardPositionに対応する方向
inline const BoardDirection GetBoardDirection(const BoardPosition board_position);

// --- BoardPosition => MovePosition/(x, y) ---
//! @brief BoardPositionからMovePositionを求める
//! @param board_position BoardPosition
//! @retval BoardPositionに対応するMovePosition
inline const MovePosition GetBoardMove(const BoardPosition board_position);

//! @brief BoardPositionから(x, y)座標を求める
//! @param board_position BoardPosition
//! @param x x座標
//! @param y y座標
//! @note x, yともに[0, 15]であることを保証する
inline const void GetBoardCordinate(const BoardPosition board_position, Cordinate * const x, Cordinate * const y);

//! @brief board_positionが盤内か判定する
//! @retval true board_positionが盤内
inline const bool IsInBoard(const BoardPosition board_position);

}   // namespace realcore

#include "Conversion-inl.h"

#endif    // COVERSION_H
