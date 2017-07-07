#ifndef SGF_PARSER_INL_H
#define SGF_PARSER_INL_H

#include "SGFParser.h"

namespace realcore{

inline const std::string SGFParser::GetGameDate() const{
  return game_date_;
}

inline const std::string SGFParser::GetBlackPlayerName() const{
  return black_player_name_;
}

inline const std::string SGFParser::GetBlackPlayerRank() const{
  return black_player_rank_;
}

inline const std::string SGFParser::GetWhitePlayerName() const{
  return white_player_name_;
}

inline const std::string SGFParser::GetWhitePlayerRank() const{
  return white_player_rank_;
}

inline const std::string SGFParser::GetGameRule() const{
  return game_rule_;
}

inline const GameEndStatus SGFParser::GetEndStatus() const{
  return game_end_status_;
}

inline const GameResult SGFParser::GetGameResult() const{
  return game_result_;
}

inline const std::string SGFParser::GetGameRecord() const{
  return game_record_;
}

inline const std::string SGFParser::GetAlternativeMoves() const{
  return alternative_moves_;
}

inline const std::string SGFParser::GetEventName() const{
  return event_name_;
}
}


#endif  // SGF_PARSER_INL_H
