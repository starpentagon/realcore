#include <iostream>
#include <set>

#include <boost/program_options.hpp>

#include "InfluenceAreaCheck.h"
#include "CSVReader.h"
#include "Board.h"
#include "HashTable.h"

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

  bool is_help = arg_map.count("help") || !arg_map.count("db");

  if(is_help){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << option;
    cout << endl;
    return 0;
  }

  // 棋譜データベースの読込
  const string diagram_db_file = arg_map["db"].as<string>();

  cerr << "Read game_record DB: " << diagram_db_file << endl;

  map<string, StringVector> diagram_db;
  ReadCSV(diagram_db_file, &diagram_db);

  const auto board_string_list = diagram_db["game_record"];
  const auto record_count = board_string_list.size();
  cerr << "Game count: " << record_count << endl;

  // LineNeighborhood::ForbiddenCheckの影響領域チェック
  cerr << "LineNeighborhood::ForbiddenCheck" << endl;
  map<string, SearchCounter> line_neighborhood_forbidden_check;
  set<HashValue> checked_board;

  for(size_t i=0; i<record_count; ++i){
    const auto& board_string = board_string_list[i];
    MoveList board_move_sequence(board_string);
    MoveList board_move;

    for(const auto move : board_move_sequence){
      if(board_move.IsBlackTurn()){
        const auto hash_value = CalcHashValue(board_move);

        if(checked_board.find(hash_value) == checked_board.end()){
          LineNeighborhoodForbiddenCheck(board_move, &line_neighborhood_forbidden_check);
          checked_board.insert(hash_value);
        }
      }

      board_move += move;
    }

    if(i % 1000 == 0){
      cerr << i << endl;
    }
  }

  // LineNeighborhood::ForbiddenCheckの影響領域チェック結果
  cerr << "kForbiddenMove:" << endl;
  cerr << "  Exact: " << line_neighborhood_forbidden_check[kCheckExactForbidden] << endl;
  cerr << "  Calc: " << line_neighborhood_forbidden_check[kCheckCalcForbidden] << endl;
  cerr << "  Calc/Exact: " << 1.0 * line_neighborhood_forbidden_check[kCheckCalcForbidden] / line_neighborhood_forbidden_check[kCheckExactForbidden] << endl;
  cerr << "kPossibleForbiddenMove:" << endl;
  cerr << "  Exact: " << line_neighborhood_forbidden_check[kCheckExactPossibleForbidden] << endl;
  cerr << "  Calc: " << line_neighborhood_forbidden_check[kCheckCalcPossibleForbidden] << endl;
  cerr << "  Calc/Exact: " << 1.0 * line_neighborhood_forbidden_check[kCheckCalcPossibleForbidden] / line_neighborhood_forbidden_check[kCheckExactPossibleForbidden] << endl;
  cerr << "kNonForbiddenMove:" << endl;
  cerr << "  Exact: " << line_neighborhood_forbidden_check[kCheckExactNonForbidden] << endl;
  cerr << "  Calc: " << line_neighborhood_forbidden_check[kCheckCalcNonForbidden] << endl;
  cerr << "  Calc/Exact: " << 1.0 * line_neighborhood_forbidden_check[kCheckCalcNonForbidden] / line_neighborhood_forbidden_check[kCheckExactNonForbidden] << endl;

  return 0;
}

void LineNeighborhoodForbiddenCheck(const realcore::MoveList &board_sequence, std::map<std::string, realcore::SearchCounter> *check_result)
{
  const bool is_black_turn = board_sequence.IsBlackTurn();

  if(!is_black_turn){
    return;
  }

  BitBoard bit_board(board_sequence);
  
  MoveList candidate_move;
  board_sequence.GetOpenMove(&candidate_move);

  for(const auto move : candidate_move){
    constexpr size_t kForbiddenCheck = 5;
    LineNeighborhood line_neighbor(move, kForbiddenCheck, bit_board);

    // すべてが空点の場合は結果が自明なためテスト対象外とする
    if(line_neighbor.IsAllOpenPosition()){
      continue;
    }

    line_neighbor.SetCenterState<kBlackStone>();

    vector<BoardPosition> next_open_four_list;
    MoveBitSet influence_area;
    const ForbiddenCheckState forbidden_state = line_neighbor.ForbiddenCheck(&next_open_four_list, &influence_area);

    // 近傍空点に石を追加して結果が変わる領域を求める
    MoveList neighbor_open_move;
    line_neighbor.GetOpenMovePosition(&neighbor_open_move);

    MoveBitSet exact_influence_area;

    if(forbidden_state == kForbiddenMove || forbidden_state == kPossibleForbiddenMove){
      exact_influence_area.set(move);
    }

    for(const auto check_move : neighbor_open_move){
      {
      // 黒石追加
      bit_board.SetState<kBlackStone>(check_move);

      LineNeighborhood check_neighbor(move, kForbiddenCheck, bit_board);
      check_neighbor.SetCenterState<kBlackStone>();
      
      vector<BoardPosition> next_open_four_list;
      const ForbiddenCheckState check_state = check_neighbor.ForbiddenCheck(&next_open_four_list);
      
      bool is_influenced = forbidden_state == kForbiddenMove && check_state != kForbiddenMove;
      is_influenced |= forbidden_state == kPossibleForbiddenMove && check_state == kNonForbiddenMove;
      is_influenced |= forbidden_state == kNonForbiddenMove && check_state != kNonForbiddenMove;

      if(is_influenced){
        exact_influence_area.set(check_move);
      }

      bit_board.SetState<kOpenPosition>(check_move);
      }
      {
      // 白石追加
      bit_board.SetState<kWhiteStone>(check_move);

      LineNeighborhood check_neighbor(move, kForbiddenCheck, bit_board);
      check_neighbor.SetCenterState<kBlackStone>();
      
      vector<BoardPosition> next_open_four_list;
      const ForbiddenCheckState check_state = check_neighbor.ForbiddenCheck(&next_open_four_list);

      bool is_influenced = forbidden_state == kForbiddenMove && check_state != kForbiddenMove;
      is_influenced |= forbidden_state == kPossibleForbiddenMove && check_state == kNonForbiddenMove;
      is_influenced |= forbidden_state == kNonForbiddenMove && check_state != kNonForbiddenMove;

      if(is_influenced){
        exact_influence_area.set(check_move);
      }

      bit_board.SetState<kOpenPosition>(check_move);
      }
    }

    // 算出した影響領域が正確な影響領域を包含していることをチェック
    const bool is_include = ((influence_area | exact_influence_area) == influence_area);

    if(!is_include){
      MoveList influence_move, exact_influence_move;
      GetMoveList(influence_area, &influence_move);
      GetMoveList(exact_influence_area, &exact_influence_move);
      cout << "[Not Include]board: " << board_sequence.str() << " , Move: " << MoveString(move) << ", ";
      cout << "exact: " << exact_influence_move.str() << " , calc: " << influence_move.str() << endl;
    }

    if(forbidden_state == kForbiddenMove){
      (*check_result)[kCheckExactForbidden] += exact_influence_area.count();
      (*check_result)[kCheckCalcForbidden] += influence_area.count();
    }else if(forbidden_state == kPossibleForbiddenMove){
      (*check_result)[kCheckExactPossibleForbidden] += exact_influence_area.count();
      (*check_result)[kCheckCalcPossibleForbidden] += influence_area.count();
    }else{
      (*check_result)[kCheckExactNonForbidden] += exact_influence_area.count();
      (*check_result)[kCheckCalcNonForbidden] += influence_area.count();
    }
  }
}
