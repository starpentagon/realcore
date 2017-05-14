//! @file
//! @brief 空点状態の定義
//! @author Koichi NABETANI
//! @date 2017/04/04
#ifndef OPEN_STATE_H
#define OPEN_STATE_H

#include <array>
#include <vector>
#include <bitset>

#include "RealCore.h"

namespace realcore{
//! @brief 空点状態の対象となる指し手パターン
enum OpenStatePattern : std::uint8_t
{
  kNextOverline,            //!< 長連点, B[B3O1]B
  kNextOpenFourBlack,       //!< 達四点(黒), XO[B3O1]OX
  kNextOpenFourWhite,       //!< 達四点(白),  O[W3O1]O
  kNextFourBlack,           //!< 四ノビ点(黒), X[B3O2]X
  kNextFourWhite,           //!< 四ノビ点(白),  [W3O2]
  kNextSemiThreeBlack,      //!< 見かけの三ノビ点(黒), XO[B2O2]OX
  kNextSemiThreeWhite,      //!< 見かけの三ノビ点(白),  O[W2O2]O
  kNextPointOfSwordBlack,   //!< 剣先点(黒), X[B2O3]X
  kNextPointOfSwordWhite,   //!< 剣先点(白),  [W2O3]
  kNextTwoBlack,            //!< 二ノビ点(黒), XO[B1O3]OX
  kNextTwoWhite,            //!< 二ノビ点(白),  O[W1O3]O
};

constexpr size_t kOpenStatePatternNum = 11;
const std::array<OpenStatePattern, kOpenStatePatternNum>& GetAllOpenStatePattern();

typedef std::bitset<kOpenStatePatternNum> UpdateOpenStateFlag;    // 更新を行う空点情報のフラグ

// 指し手パターンの生成フラグ定数
static constexpr uint64_t kUpdateFlagForbidden = 0b00000101011;           //!< 禁手チェック用(黒番の長連点, 達四点, 四ノビ点, 見かけの三ノビ点)
static constexpr uint64_t kUpdateFlagOpenFourWhite = 0b00000000100;       //!< 達四点(白)
static constexpr uint64_t kUpdateFlagFourWhite = 0b00000010000;           //!< 四ノビ点(白)
static constexpr uint64_t kUpdateFlagSemiThreeWhite = 0b00001000000;      //!< 見かけの三ノビ点(白)
static constexpr uint64_t kUpdateFlagPointOfSwordBlack = 0b00010000000;   //!< 剣先点(黒)
static constexpr uint64_t kUpdateFlagPointOfSwordWhite = 0b00100000000;   //!< 剣先点(白)
static constexpr uint64_t kUpdateFlagTwoBlack = 0b01000000000;            //!< 二ノビ点(黒)
static constexpr uint64_t kUpdateFlagTwoWhite = 0b10000000000;            //!< 二ノビ点(白)

static constexpr uint64_t kUpdateAllFlag = 0b11111111111;

constexpr UpdateOpenStateFlag kUpdateAllOpenState(kUpdateAllFlag);      // すべての空点情報を更新する
constexpr UpdateOpenStateFlag kUpdateForbiddenCheck(kUpdateFlagForbidden);    // 禁手チェック用の空点情報を更新する

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
  template<PlayerTurn P>
  void GetInfluenceArea(std::vector<BoardPosition> * const downward_influence_area) const;

  //! @brief 剣先点における四ノビ位置([B203][W2O3]のopen_position以外のO)を返す
  void GetFourPosition(std::array<BoardPosition, 2> * const four_position_list) const;

  //! @brief 二ノビ点における見かけ三になる位置([B1O3][W1O3]のopen_position以外のO)を返す
  void GetSemiThreePosition(std::array<BoardPosition, 2> * const semi_three_position_list) const;

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
