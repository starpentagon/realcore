#include <iostream>
#include <memory>
#include <chrono>

#include <boost/program_options.hpp>

#include "EnumerateForbiddenMove.h"
#include "CSVReader.h"
#include "Board.h"

using namespace std;
using namespace boost::program_options;
using namespace realcore;

int main(int argc, char* argv[])
{
  // オプション設定
  options_description option;

  option.add_options()
    ("db", value<string>(), "棋譜データベース(csv)")
    ("log", "列挙結果を出力する")
    ("mode", value<string>()->default_value("point"), "point:各点チェック, enum:列挙法, enum-diff:差分法列挙")
    ("help,h", "ヘルプを表示");
  
  variables_map arg_map;
  store(parse_command_line(argc, argv, option), arg_map);

  const auto mode = arg_map["mode"].as<string>();
  bool is_help = arg_map.count("help") || !arg_map.count("db");
  is_help |= !(mode == "point" || mode == "enum" || mode == "enum-diff");

  if(is_help){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << option;
    cout << endl;

    return 0;
  }

  cerr << "Mode: " << mode << endl;

  // 棋譜データベースの読込
  const string diagram_db_file = arg_map["db"].as<string>();

  cerr << "Read game_record DB: " << diagram_db_file << endl;

  map<string, StringVector> diagram_db;
  ReadCSV(diagram_db_file, &diagram_db);

  const auto board_str_list = diagram_db["game_record"];

  vector< shared_ptr<MoveList> > board_move_list;
  board_move_list.reserve(board_str_list.size());

  for(const auto board_str : board_str_list){
    auto move_list = make_shared<MoveList>(board_str);
    board_move_list.push_back(move_list);
  }

  cerr << "Game count: " << board_str_list.size() << endl;;

  // 禁手の列挙
  cerr << "Enumerate forbidden moves" << endl;
  
  vector< shared_ptr<MoveList> > forbidden_board_list, forbidden_move_list;
  auto start_time = chrono::system_clock::now();
  size_t board_count = 0, forbidden_count = 0;
  const bool is_output_result = arg_map.count("log");

  for(const auto& move_list : board_move_list){
    MoveList board_move;
    Board board;
    BitBoard bit_board;
    bool is_black_turn = true;

    for(const auto move : (*move_list)){
      MoveList forbidden_move;
      board_move += move;

      if(mode == "enum-diff"){
        board.MakeMove(move);
        EnumerateOpenState(board, &forbidden_move);
      }else if(mode == "enum"){
        if(is_black_turn){
          bit_board.SetState<kBlackStone>(move);
        }else{
          bit_board.SetState<kWhiteStone>(move);
          EnumerateOpenState(bit_board, &forbidden_move);
        }
  
        is_black_turn = !is_black_turn;
      }else{
        if(is_black_turn){
          bit_board.SetState<kBlackStone>(move);
        }else{
          bit_board.SetState<kWhiteStone>(move);
          CheckEachPoint(board_move, &forbidden_move);
        }
  
        is_black_turn = !is_black_turn;
      }

      forbidden_count += forbidden_move.size();

      if(is_output_result && !forbidden_move.empty()){
        // 禁手が存在する場合は盤面と禁手リストを記録する
        auto forbidden_board = make_shared<MoveList>(board_move);
        auto forbidden = make_shared<MoveList>(forbidden_move);

        forbidden_board_list.push_back(forbidden_board);
        forbidden_move_list.push_back(forbidden);
      }

      board_count++;
    }
  }

  auto elapsed_time = chrono::system_clock::now() - start_time;
  cerr << "Time: " << chrono::duration_cast<chrono::milliseconds>(elapsed_time).count() << endl;
  cerr << "Board count: " << board_count << endl;
  cerr << "Forbidden moves: " << forbidden_count << endl;

  for(size_t i=0, size=forbidden_board_list.size(); i<size; i++){
    cout << forbidden_board_list[i]->str() << ",";
    cout << forbidden_move_list[i]->str() << endl;
  }

  return 0;
}

void CheckEachPoint(const BitBoard &bit_board, MoveList * const forbidden_move)
{
  for(const auto move : GetAllInBoardMove())
  {
    auto state = bit_board.GetState(move);

    if(state != kOpenPosition){
      continue;
    }

    if(bit_board.IsForbiddenMove<kBlackTurn>(move)){
      (*forbidden_move) += move;
    }
  }
}

void EnumerateOpenState(const Board &board, MoveList * const forbidden_move)
{
  MoveBitSet forbidden_bit_set;
  board.EnumerateForbiddenMoves(&forbidden_bit_set);

  for(const auto move : GetAllInBoardMove()){
    if(forbidden_bit_set[move]){
      (*forbidden_move) += move;
    }
  }
}

void EnumerateOpenState(const BitBoard &bit_board, MoveList * const forbidden_move)
{
  MoveBitSet forbidden_bit_set;
  bit_board.EnumerateForbiddenMoves(&forbidden_bit_set);

  for(const auto move : GetAllInBoardMove()){
    if(forbidden_bit_set[move]){
      (*forbidden_move) += move;
    }
  }
}
