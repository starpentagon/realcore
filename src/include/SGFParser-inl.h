#ifndef SGF_PARSER_INL_H
#define SGF_PARSER_INL_H

#include "SGFParser.h"

namespace realcore{

inline std::string SGFParser::GetGameDate() const{
  return game_date_;
}

inline std::string SGFParser::GetBlackPlayerName() const{
  return black_player_name_;
}

inline std::string SGFParser::GetBlackPlayerRank() const{
  return black_player_rank_;
}

inline std::string SGFParser::GetWhitePlayerName() const{
  return white_player_name_;
}

inline std::string SGFParser::GetWhitePlayerRank() const{
  return white_player_rank_;
}

inline std::string SGFParser::GetGameRule() const{
  return game_rule_;
}

inline GameEndStatus SGFParser::GetEndStatus() const{
  return game_end_status_;
}

inline GameResult SGFParser::GetGameResult() const{
  return game_result_;
}

inline std::string SGFParser::GetGameRecord() const{
  return game_record_;
}

inline std::string SGFParser::GetAlternativeMoves() const{
  return alternative_moves_;
}

inline std::string SGFParser::GetEventName() const{
  return event_name_;
}
}


#endif  // SGF_PARSER_INL_H
