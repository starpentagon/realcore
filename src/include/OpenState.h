//! @file
//! @brief 空点状態の定義
//! @author Koichi NABETANI
//! @date 2017/04/04
#ifndef OPEN_STATE_H
#define OPEN_STATE_H

#include <vector>
#include <bitset>

#include "RealCore.h"

namespace realcore{
//! @brief 空点状態の対象となる指し手パターン
enum OpenStatePattern : std::uint8_t
{
  kNextOverline,          //!< 長連点 
  kNextOpenFourBlack,     //!< 達四点(黒)
  kNextOpenFourWhite,     //!< 達四点(白)
  kNextFourBlack,         //!< 四ノビ点(黒)
  kNextFourWhite,         //!< 四ノビ点(白)
  kNextSemiThreeBlack,    //!< 見かけの三ノビ点(黒)
  kNextSemiThreeWhite,    //!< 見かけの三ノビ点(白)
};

constexpr size_t kOpenStatePatternNum = 7;
const std::array<OpenStatePattern, kOpenStatePatternNum>& GetAllOpenStatePattern();

typedef std::bitset<kOpenStatePatternNum> UpdateOpenStateFlag;    // 更新を行う空点情報のフラグ

constexpr UpdateOpenStateFlag kUpdateAllOpenState(0b1111111);    // すべての空点情報を更新する
constexpr UpdateOpenStateFlag kUpdateForbiddenCheck(0b0101011);    // 禁手チェック用の空点情報を更新する

//! @brief 指し手パターンが黒番, 白番どちらのパターンなのかを返す
constexpr PlayerTurn GetPatternPlayerTurn(const OpenStatePattern pattern);

// 前方宣言
class OpenState;
class OpenStateTest;
enum MovePosition : std::uint8_t;

//! @brief 2つの空点状態を比較する
//! @retval true 2つの空点状態が同一の内容を保持
bool IsEqual(const OpenState &lhs, const OpenState &rhs);

//! @brief コピーを作成する
//! @param bit_board_from コピー元
//! @param bit_board_to コピー先
void Copy(const OpenState &from, OpenState * const to);

//! @brief 空点状態を管理するクラス
class OpenState{
  friend class OpenStateTest;
  friend bool IsEqual(const OpenState &lhs, const OpenState &rhs);
  friend void Copy(const OpenState &from, OpenState * const to);

public:
  //! @brief コンストラクタ
  OpenState(const OpenStatePattern pattern, const BoardPosition open_position, const BoardPosition pattern_position, const size_t pattern_search_index);
  OpenState(const OpenState &open_state);

  //! @brief 代入演算子
  const OpenState& operator=(const OpenState &open_state);

  //! @brief 比較演算子
  const bool operator==(const OpenState &open_state) const;
  const bool operator!=(const OpenState &open_state) const;

  //! @brief 空点の盤面位置を返す
  const BoardPosition GetOpenPosition() const;

  //! @brief パターンの開始位置を返す
  const BoardPosition GetPatternPosition() const;

  //! @brief チェック対象位置（見かけの三点における達四点 or 四ノビ点における五連を作る位置）を返す
  const BoardPosition GetCheckPosition() const;

  //! @brief パターンの指し手が成立しなくなる影響領域(downward influence area)を返す
  //! @note 長連筋の位置は空点でない可能性あるため着手前に空点チェックが必要
  template<PlayerTurn P>
  void GetInfluenceArea(std::vector<BoardPosition> * const downward_influence_area) const;

  //! @brief 指定の指し手位置が影響領域かどうかを判定する
  //! @param P 指し手の手番
  //! @param move 指し手位置
  //! @retval true 影響領域に入る
  template<PlayerTurn P>
  const bool IsInfluenceMove(const MovePosition move) const;

private:
  OpenStatePattern pattern_;              //!< 空点状態の対象となる指し手パターン(長連点, 達四点, etc)
  BoardPosition open_position_;           //!< 空点位置
  BoardPosition pattern_position_;        //!< パターンの開始位置
  size_t pattern_search_index_;           //!< パターン検索index
};
}   // namespace realcore

#include "OpenState-inl.h"

#endif    // OPEN_STATE_H
