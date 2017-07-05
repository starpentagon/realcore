#ifndef SGF_READER_INL_H
#define SGF_READER_INL_H

#include "SGFReader.h"

namespace realcore{

inline std::string SGFReader::GetGameDate() const{
  return game_date_;
}

inline std::string SGFReader::GetBlackPlayerName() const{
  return black_player_name_;
}

inline std::string SGFReader::GetBlackPlayerRank() const{
  return black_player_rank_;
}

inline std::string SGFReader::GetWhitePlayerName() const{
  return white_player_name_;
}

inline std::string SGFReader::GetWhitePlayerRank() const{
  return white_player_rank_;
}

inline std::string SGFReader::GetGameRule() const{
  return game_rule_;
}

inline GameEndStatus SGFReader::GetEndStatus() const{
  return game_end_status_;
}

inline GameResult SGFReader::GetGameResult() const{
  return game_result_;
}

inline std::string SGFReader::GetGameRecord() const{
  return game_record_;
}

inline std::string SGFReader::GetAlternativeMoves() const{
  return alternative_moves_;
}

inline std::string SGFReader::GetEventName() const{
  return event_name_;
}
}


#endif  // SGF_READER_INL_H
