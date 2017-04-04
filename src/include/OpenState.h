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

  // 前方宣言
  class OpenStateTest;
  enum MovePosition : std::uint8_t;

  //! @brief 空点状態を管理するクラス
  template<OpenStatePattern>
  class OpenState{
    friend class OpenStateTest;

  public:
    //! @brief コンストラクタ
    OpenState(const BoardPosition pattern_position);

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
    //! @param move 指し手位置
    //! @retval true 影響領域に入る
    const bool IsInfluenceMove(const MovePosition move) const;

  private:
    BoardPosition pattern_position_;    //!< パターンの開始位置
    std::vector<BoardPosition> check_position_list_;    //!< チェック対象位置（見かけの三の四連にする位置）
    std::vector<BoardPosition> guard_position_list_;    //!< 防手位置
  };
}   // namespace realcore

#include "OpenState-inl.h"

#endif    // OPEN_STATE_H
