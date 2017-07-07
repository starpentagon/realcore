#include <iostream>
#include <set>
#include <regex>

#include "MoveList.h"
#include "SGFParser.h"

using namespace std;
using namespace realcore;

void realcore::GetMoveListFromSGFData(const SGFCheckBit &check_bit, const std::string &sgf_data, MoveList * move_list)
{
  SGFParser sgf_parser(check_bit);
  sgf_parser.ParseSGF(sgf_data);

  const auto diagram_str = sgf_parser.GetGameRecord();
  GetMoveList(diagram_str, move_list);
}

SGFParser::SGFParser(const SGFCheckBit &check_bit)
: game_end_status_(kUnknownEndStatus), game_result_(kUnknownResult), check_bit_(check_bit)
{
}

void SGFParser::ParseSGF(const string &sgf_data)
{
  try{
    game_date_ = ParseGameDate(sgf_data);
    event_name_ = ParseEventName(sgf_data);
    black_player_name_ = ParseBlackPlayerName(sgf_data);
    black_player_rank_ = ParseBlackPlayerRank(sgf_data); 
    white_player_name_ = ParseWhitePlayerName(sgf_data);
    white_player_rank_ = ParseWhitePlayerRank(sgf_data);

    game_rule_ = ParseGameRule(sgf_data);
    game_end_status_ = ParseGameEndStatus(sgf_data);
    game_result_ = ParseGameResult(sgf_data);
    game_record_ = ParseGameRecord(sgf_data);
    alternative_moves_ = ParseAlternativeMoves(sgf_data);
  }catch(logic_error &ex){
    throw ex;
  }
}

const string SGFParser::ParseGameDate(const std::string &sgf_data) const
{
  static regex date_expr("DT\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), date_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else if(check_bit_[kSGFDateCheck]){
    logic_error error("Date(DT) is not found.");
    throw error;
  }else{
    return "";
  }
}

const string SGFParser::ParseBlackPlayerName(const std::string &sgf_data) const
{
  static regex black_player_name_expr("PB\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), black_player_name_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else if(check_bit_[kSGFBlackPlayerNameCheck]){
    logic_error error("Black Player Name(PB) is not found.");
    throw error;
  }else{
    return "";
  }
}

const string SGFParser::ParseBlackPlayerRank(const std::string &sgf_data) const
{
  static regex black_player_rank_expr("BR\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), black_player_rank_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else{
    return "";
  }
}

const string SGFParser::ParseWhitePlayerName(const std::string &sgf_data) const
{
  static regex white_player_name_expr("PW\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), white_player_name_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else if(check_bit_[kSGFWhitePlayerNameCheck]){
    logic_error error("White Player Name(PW) is not found.");
    throw error;
  }else{
    return "";
  }
}

const string SGFParser::ParseWhitePlayerRank(const std::string &sgf_data) const
{
  static regex white_player_rank_expr("WR\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), white_player_rank_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else{
    return "";
  }
}

const string SGFParser::ParseGameRule(const std::string &sgf_data) const
{
  static regex game_rule_expr("(GN|RU)\\[.*?((RIF)|(Sakata)|(Yamaguchi)|(Tarannikov)|(Jonsson)|(Unknown)).*?\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), game_rule_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(2);
  }else{
    return "Unknown";
  }
}

const GameEndStatus SGFParser::ParseGameEndStatus(const std::string &sgf_data) const
{
  static regex draw_expr("RE\\[B?W?\\+?Draw\\]");
  sregex_iterator it_draw(sgf_data.begin(), sgf_data.end(), draw_expr);
  sregex_iterator it_end;

  if(it_draw != it_end){
    return kAgreedDraw;
  }

  static regex result_expr("RE\\[(B|W)\\+(Resign|R|Time)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), result_expr);

  if(it != it_end){
    const string end_status = it->str(2);

    if(end_status == "Resign" || end_status == "R"){
      return kResign;
    }else{
      return kTimeup;
    }
  }else if(check_bit_[kSGFGameResultCheck]){
    logic_error error("Game result(RE) is not found.");
    throw error;
  }else{
    return kUnknownEndStatus;
  }
}

const GameResult SGFParser::ParseGameResult(const std::string &sgf_data) const
{
  if(ParseGameEndStatus(sgf_data) == kAgreedDraw){
    return kDraw;
  }

  static regex result_expr("RE\\[(B|W)\\+(Resign|R|Time)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), result_expr);
  sregex_iterator it_end;

  if(it != it_end){
    const string win_player = it->str(1);

    if(win_player == "B"){
      return kBlackWin;
    }else{
      return kWhiteWin;
    }
  }else if(check_bit_[kSGFGameResultCheck]){
    logic_error error("Game result(RE) is not found.");
    throw error;
  }else{
    return kUnknownResult;
  }
}

const bool SGFParser::IsValidMove(const std::string &move) const
{
  if(move == "tt"){
    return true;
  }

  bool is_valid_move = move.at(0) >= 'a' && move.at(0) <= 'o';
  is_valid_move &= move.at(1) >= 'a' && move.at(1) <= 'o';

  return is_valid_move;
}

const string SGFParser::ParseInitBoard(const std::string &sgf_data) const
{
  // 黒石位置、白石位置の取得
  static regex stone_expr("[a-z][a-z]");
  sregex_iterator it_end;

  vector<string> black_stone_vec;
  static regex black_stone_expr("AB(\\[[a-z][a-z]\\])+");
  sregex_iterator black_stone_it(sgf_data.begin(), sgf_data.end(), black_stone_expr);

  if(black_stone_it != it_end){
    const auto init_board_str = black_stone_it->str();
    sregex_iterator stone_it(init_board_str.begin(), init_board_str.end(), stone_expr);

    for(; stone_it!=it_end; ++stone_it){
      black_stone_vec.emplace_back(stone_it->str());
    }
  }

  vector<string> white_stone_vec;
  static regex white_stone_expr("AW(\\[[a-z][a-z]\\])+");
  sregex_iterator white_stone_it(sgf_data.begin(), sgf_data.end(), white_stone_expr);

  if(white_stone_it != it_end){
    const auto init_board_str = white_stone_it->str();
    sregex_iterator stone_it(init_board_str.begin(), init_board_str.end(), stone_expr);

    for(; stone_it!=it_end; ++stone_it){
      white_stone_vec.emplace_back(stone_it->str());
    }
  }

  // 黒番、白番の取得
  const size_t black_size = black_stone_vec.size();
  const size_t white_size = white_stone_vec.size();

  bool is_black_turn = true;
  
  if(black_size == white_size){
    is_black_turn = true;
  }

  if(black_size == white_size + 1){
    is_black_turn = false;
  }

  static regex black_turn_expr("PL\\[B\\]");
  sregex_iterator black_turn_it(sgf_data.begin(), sgf_data.end(), black_turn_expr);

  if(black_turn_it != it_end){
    is_black_turn = true;
  }

  static regex white_turn_expr("PL\\[W\\]");
  sregex_iterator white_turn_it(sgf_data.begin(), sgf_data.end(), white_turn_expr);

  if(white_turn_it != it_end){
    is_black_turn = false;
  }

  // 手番に応じた石数にする
  if(is_black_turn){
    if(black_size > white_size){
      const auto diff_size = black_size - white_size;

      for(size_t i=0; i<diff_size; i++){
        white_stone_vec.emplace_back("pp");
      }
    }else if(black_size < white_size){
      const auto diff_size = white_size - black_size;

      for(size_t i=0; i<diff_size; i++){
        black_stone_vec.emplace_back("pp");
      }
    }
  }else{
    if(black_size > white_size + 1){
      const auto diff_size = black_size - white_size - 1;

      for(size_t i=0; i<diff_size; i++){
        white_stone_vec.emplace_back("pp");
      }
    }else if(black_size < white_size + 1){
      const auto diff_size = white_size + 1 - black_size;

      for(size_t i=0; i<diff_size; i++){
        black_stone_vec.emplace_back("pp");
      }
    }
  }

  // 出力
  string init_board_string;

  for(size_t i=0, size=white_stone_vec.size(); i<size; i++){
    init_board_string += black_stone_vec[i];
    init_board_string += white_stone_vec[i];
  }

  if(!is_black_turn){
    const auto last_index = black_stone_vec.size() - 1;
    init_board_string += black_stone_vec[last_index];
  }

  return init_board_string;
}

const string SGFParser::ParseGameRecord(const std::string &sgf_data) const
{
  static regex move_expr(";(B|W)\\[([a-z][a-z])\\]");

  sregex_iterator move_it(sgf_data.begin(), sgf_data.end(), move_expr);
  sregex_iterator it_end;
  
  set<string> move_set;
  bool black_turn = true;
  string record = ParseInitBoard(sgf_data);

  while(move_it != it_end)
  {
    const string turn = move_it->str(1);
    const string stone = move_it->str(2);
    
    // 黒番、白番が交互に出現しているかチェックする
    bool is_turn_consistent = (black_turn && turn == "B") || (!black_turn && turn == "W");

    if(!is_turn_consistent){
      throw logic_error("Turn is not consistent.");
    }

    // timeup or pass
    if(stone == "tt"){
      break;
    }

    if(!IsValidMove(stone)){
      const string error_message = "Illegal move: " + stone;
      logic_error error(error_message);
      throw error;
    }

    // 同じ場所への着手がある場合が稀にあるのでチェック
    set<string>::const_iterator find_it = move_set.find(stone);

    if(find_it != move_set.end()){
      const string error_message = "Conflict move: " + stone;
      logic_error error(error_message);
      throw error;
    }

    record += stone;

    ++move_it;
    black_turn = !black_turn;
  }

  return record;
}

const string SGFParser::ParseAlternativeMoves(const std::string &sgf_data) const
{
  static regex alternative_expr("(5A|FA)\\[([a-o0-9\\s\\[\\]]*)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), alternative_expr);
  sregex_iterator it_end;

  if(it != it_end){
    string alternative_move = it->str(2);

    replace(alternative_move.begin(), alternative_move.end(), '[', ' ');
    replace(alternative_move.begin(), alternative_move.end(), ']', ';');

    return alternative_move;
  }else{
    return "";
  }
}

const string SGFParser::ParseEventName(const std::string &sgf_data) const
{
  static regex event_expr("EV\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), event_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else{
    return "";
  }
}


