//! @file
//! @brief 1局分のSGFデータを読み込むクラス
//! @author Koichi NABETANI
//! @date 2017/07/05
#ifndef SGF_PARSER_H
#define SGF_PARSER_H

#include <string>

namespace realcore
{

//! @brief 対局結果
enum GameResult{
  kBlackWin,        //!< 黒勝
  kWhiteWin,        //!< 白勝
  kDraw,            //!< 満局
  kUnknownResult,   //!< 不明
};

//! @brief 終局ステータス
enum GameEndStatus
{
  kResign,            //!< 投了
  kTimeup,            //!< 時間切れ
  kAgreedDraw,        //!< 合意満局
  kUnknownEndStatus,  //!< 不明
};

class MoveList;

//! @brief SGFデータの指し手リストを生成する
void GetMoveListFromSGFData(const std::string &sgf_data, MoveList * move_list);

//! @brief 1局分のSGFデータを読み込み対局情報を保持するクラス
class SGFParserTest;

class SGFParser{
  friend class SGFParserTest;

public:
  SGFParser();
  
  //! @param sgf_data SGFデータ
  //! @note 読み込みに失敗した場合はlogic_error例外を送出する
  void ParseSGF(const std::string &sgf_data);

  std::string GetGameDate() const;

  std::string GetBlackPlayerName() const;
  std::string GetBlackPlayerRank() const;
  
  std::string GetWhitePlayerName() const;
  std::string GetWhitePlayerRank() const;

  std::string GetGameRule() const;
  GameEndStatus GetEndStatus() const;
  GameResult GetGameResult() const;

  std::string GetGameRecord() const;
  std::string GetAlternativeMoves() const;

  std::string GetEventName() const;

private:
  //! @brief 対局日を取得する
  std::string ParseGameDate(const std::string &sgf_data) const;

  //! @brief 黒番の対局者名を取得する
  std::string ParseBlackPlayerName(const std::string &sgf_data) const;

  //! @brief 黒番のランク(1dなど)を取得する
  std::string ParseBlackPlayerRank(const std::string &sgf_data) const;

  //! @brief 白番の対局者名を取得する
  std::string ParseWhitePlayerName(const std::string &sgf_data) const;

  //! @brief 白番のランク(1dなど)を取得する
  std::string ParseWhitePlayerRank(const std::string &sgf_data) const;

  //! @brief 対局ルールを取得する
  std::string ParseGameRule(const std::string &sgf_data) const;

  //! @brief 終局状態を取得する
  GameEndStatus ParseGameEndStatus(const std::string &sgf_data) const;

  //! @brief 対局結果を取得する
  GameResult ParseGameResult(const std::string &sgf_data) const;

  //! @brief 棋譜([a-o]形式)を取得する
  std::string ParseGameRecord(const std::string &sgf_data) const;

  //! @brief 5手目の取り除かれた提示珠を取得する
  std::string ParseAlternativeMoves(const std::string &sgf_data) const;

  //! @brief 大会名を取得する
  std::string ParseEventName(const std::string &sgf_data) const;

  //! @brief CSVフィールドにカンマが入らないようにカンマをスペースに置換する
  std::string ReplaceComma(const std::string &str) const;

  std::string game_date_;     //!< 対局日

  std::string black_player_name_;   //!< 黒番の対局者名
  std::string black_player_rank_;   //!< 黒番のランク(option)
  std::string white_player_name_;   //!< 白番の対局者名
  std::string white_player_rank_;   //!< 白番のランク(option)

  std::string game_rule_;         //!< 対局ルール(RIF, Sakata, Yamaguchi, Tarannikov, Jonsson, Unknown)
  GameEndStatus game_end_status_; //!< 終局状態
  GameResult game_result_;        //!< 対局結果
  std::string game_record_;           //!< 棋譜（[a-o]形式）
  std::string alternative_moves_; //!< 5手目の提示（取り除かれた珠）(option)

  std::string event_name_;    //!< 大会名(option)
};  // class SGFParser
}   // namespace realcore

#include "SGFParser-inl.h"

#endif    // SGF_PARSER_H