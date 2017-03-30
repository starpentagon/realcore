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
    ("help,h", "ヘルプを表示");
  
  variables_map arg_map;
  store(parse_command_line(argc, argv, option), arg_map);

  if(arg_map.count("help") || !arg_map.count("db")){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << option;
    cout << endl;

    return 0;
  }

  // 棋譜データベースの読込
  const string diagram_db_file = arg_map["db"].as<string>();

  cerr << "Read diagram_db: " << diagram_db_file << endl;

  map<string, StringVector> diagram_db;
  ReadCSV(diagram_db_file, &diagram_db);

  const auto board_str_list = diagram_db["diagram"];

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

  for(const auto& move_list : board_move_list){
    MoveList board_move;

    for(const auto move : (*move_list)){
      board_move += move;
      board_count++;

      MoveList forbidden_move;
      CheckEachPoint(board_move, &forbidden_move);

      if(!forbidden_move.empty()){
        // 禁手が存在する場合は盤面と禁手リストを記録する
        auto forbidden_board = make_shared<MoveList>(board_move);
        auto forbidden = make_shared<MoveList>(forbidden_move);

        forbidden_board_list.push_back(forbidden_board);
        forbidden_move_list.push_back(forbidden);

        forbidden_count += forbidden_move.size();
      }
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

void CheckEachPoint(const realcore::MoveList &board_move, realcore::MoveList * const forbidden_move)
{
  if(!board_move.IsBlackTurn()){
    return;
  }

  Board board(board_move);

  for(const auto move : GetAllInBoardMove())
  {
    auto state = board.GetState(move);

    if(state != kOpenPosition){
      continue;
    }

    if(board.IsForbiddenMove<kBlackTurn>(move)){
      (*forbidden_move) += move;
    }
  }
}
