//! @file
//! @brief 空点状態の定義
//! @author Koichi NABETANI
//! @date 2017/04/04
#ifndef OPEN_STATE_H
#define OPEN_STATE_H

#include <vector>

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

//! @brief 指し手パターンが黒番, 白番どちらのパターンなのかを返す
constexpr PlayerTurn GetPatternPlayerTurn(const OpenStatePattern pattern);

// 前方宣言
template<OpenStatePattern Pattern> class OpenState;
class OpenStateTest;
enum MovePosition : std::uint8_t;

//! @brief 2つの空点状態を比較する
//! @retval true 2つの空点状態が同一の内容を保持
template<OpenStatePattern Pattern>
bool IsEqual(const OpenState<Pattern> &open_state_1, const OpenState<Pattern> &open_state_2);

//! @brief コピーを作成する
//! @param bit_board_from コピー元
//! @param bit_board_to コピー先
template<OpenStatePattern Pattern>
void Copy(const OpenState<Pattern> &open_state_from, OpenState<Pattern> * const open_state_to);

//! @brief 空点状態を管理するクラス
template<OpenStatePattern Pattern>
class OpenState{
  friend class OpenStateTest;
  friend bool IsEqual<Pattern>(const OpenState<Pattern> &open_state_1, const OpenState<Pattern> &open_state_2);
  friend void Copy<Pattern>(const OpenState<Pattern> &open_state_from, OpenState<Pattern> * const open_state_to);

public:
  //! @brief コンストラクタ
  OpenState(const BoardPosition open_position, const BoardPosition pattern_position);
  OpenState(const OpenState<Pattern> &open_state);

  //! @brief 代入演算子
  const OpenState<Pattern>& operator=(const OpenState<Pattern> &open_state);

  //! @brief 比較演算子
  const bool operator==(const OpenState<Pattern> &open_state) const;
  const bool operator!=(const OpenState<Pattern> &open_state) const;

  //! @brief 空点の盤面位置を返す
  const BoardPosition GetOpenPosition() const;

  //! @brief パターンの開始位置を返す
  const BoardPosition GetPatternPosition() const;

  //! @brief チェック対象位置のリストを取得する
  const std::vector<BoardPosition>& GetCheckPositionList() const;
  
  //! @brief チェック対象位置リストを設定する
  //! @param check_position_list 設定するチェック対象位置リスト
  void SetCheckPositionList(const std::vector<BoardPosition> &check_position_list);

  //! @brief 防手位置のリストを取得する
  const std::vector<BoardPosition>& GetGuardPositionList() const;

  //! @brief 防手位置のリストを設定する
  //! @param guard_position_list 設定する防手位置リスト
  void SetGuardPositionList(const std::vector<BoardPosition> &guard_position_list);

  //! @brief 指定の指し手位置が影響領域かどうかを判定する
  //! @param P 指し手の手番
  //! @param move 指し手位置
  //! @retval true 影響領域に入る
  template<PlayerTurn P>
  const bool IsInfluenceMove(const MovePosition move) const;

private:
  BoardPosition open_position_;                       //!< 空点位置
  BoardPosition pattern_position_;                    //!< パターンの開始位置
  std::vector<BoardPosition> check_position_list_;    //!< チェック対象位置（見かけの三の四連にする位置）
  std::vector<BoardPosition> guard_position_list_;    //!< 防手位置
};
}   // namespace realcore

#include "OpenState-inl.h"

#endif    // OPEN_STATE_H
