#include <iostream>
#include <set>

#include <boost/program_options.hpp>

#include "InfluenceAreaCheck.h"
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
    ("line", "直線近傍によるパターン判定テスト(長連/四々/見かけの三々)")
    ("dbl-three", "三々判定テスト")
    ("guard", "1手勝ちの防手テスト")
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

  const auto record_string_list = diagram_db["game_record"];
  const auto record_count = record_string_list.size();
  cerr << "Game count: " << record_count << endl;

  // LineNeighborhood::ForbiddenCheckの影響領域チェック
  if(arg_map.count("line")){
    cerr << "LineNeighborhood::ForbiddenCheck" << endl;
    LineNeighborhoodForbiddenTest(record_string_list);
  }
  
  // 三々のチェック
  if(arg_map.count("dbl-three")){
    cerr << "BitBoard::IsForbiddenMove" << endl;
    DoubleThreeTest(record_string_list);
  }

  if(arg_map.count("guard")){
    cerr << "Board::GetTerminateGuard" << endl;
    GetTerminateGuardTest(record_string_list);
  }

  return 0;
}

void LineNeighborhoodForbiddenTest(const realcore::StringVector &record_string_list)
{
  map<string, SearchCounter> line_neighborhood_forbidden_check;
  set<HashValue> checked_board;
  const auto record_count = record_string_list.size();

  for(size_t i=0; i<record_count; ++i){
    const auto& board_string = record_string_list[i];
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
    MoveBitSet downward_influence_area, upward_influence_area;
    const ForbiddenCheckState forbidden_state = line_neighbor.ForbiddenCheck(&next_open_four_list, &downward_influence_area, &upward_influence_area);

    const MoveBitSet &influence_area = (forbidden_state == kForbiddenMove || forbidden_state == kPossibleForbiddenMove) ? downward_influence_area : upward_influence_area;

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

void DoubleThreeTest(const realcore::StringVector &record_string_list)
{
  map<string, SearchCounter> double_three_forbidden_check;
  set<HashValue> checked_board;
  const auto record_count = record_string_list.size();

  for(size_t i=0; i<record_count; ++i){
    const auto& board_string = record_string_list[i];
    MoveList board_move_sequence(board_string);
    MoveList board_move;

    for(const auto move : board_move_sequence){
      if(board_move.IsBlackTurn()){
        const auto hash_value = CalcHashValue(board_move);

        if(checked_board.find(hash_value) == checked_board.end()){
          DoubleThreeCheck(board_move, &double_three_forbidden_check);
          checked_board.insert(hash_value);
        }
      }

      board_move += move;
    }

    if(i % 1000 == 0){
      cerr << i << endl;
    }
  }

  // BitBoard::IsForbiddenMoveの影響領域チェック結果
  cerr << "DoubleThree:" << endl;
  cerr << "  Exact: " << double_three_forbidden_check[kExactCheckDoubleThree] << endl;
  cerr << "  Calc: " << double_three_forbidden_check[kCalcCheckDoubleThree] << endl;
  cerr << "  Calc/Exact: " << 1.0 * double_three_forbidden_check[kCalcCheckDoubleThree] / double_three_forbidden_check[kExactCheckDoubleThree] << endl;
  cerr << "NonDoubleThree:" << endl;
  cerr << "  Exact: " << double_three_forbidden_check[kExactCheckNonDoubleThree] << endl;
  cerr << "  Calc: " << double_three_forbidden_check[kCalcCheckNonDoubleThree] << endl;
  cerr << "  Calc/Exact: " << 1.0 * double_three_forbidden_check[kCalcCheckNonDoubleThree] / double_three_forbidden_check[kExactCheckNonDoubleThree] << endl;
}

void DoubleThreeCheck(const realcore::MoveList &board_sequence, std::map<std::string, realcore::SearchCounter> *check_result)
{
  const bool is_black_turn = board_sequence.IsBlackTurn();

  if(!is_black_turn){
    return;
  }

  MoveList candidate_move;
  board_sequence.GetOpenMove(&candidate_move);

  BitBoard bit_board(board_sequence);
  BoardOpenState board_open_state;
  bit_board.GetBoardOpenState(kUpdateForbiddenCheck, &board_open_state);

  MoveBitSet double_semi_three;
  bit_board.EnumerateDoubleSemiThreeMoves<kBlackTurn>(board_open_state, &double_semi_three);

  MoveList double_semi_three_move_list;
  GetMoveList(double_semi_three, &double_semi_three_move_list);

  for(const auto move : double_semi_three_move_list){
    MoveBitSet downward_influence_area, upward_influence_area;
    const bool is_forbidden = bit_board.IsForbiddenMove<kBlackTurn>(move, &downward_influence_area, &upward_influence_area);
    const MoveBitSet &influence_area = is_forbidden ? downward_influence_area : upward_influence_area;

    MoveBitSet exact_influence_area;

    if(is_forbidden){
      exact_influence_area.set(move);
    }
    
    for(const auto check_move : candidate_move){
      if(check_move == move){
        continue;
      }
      {
      bit_board.SetState<kBlackStone>(check_move);

      const bool check_forbidden = bit_board.IsForbiddenMove<kBlackTurn>(move);
      
      if(check_forbidden != is_forbidden){
        exact_influence_area.set(check_move);
      }
      
      bit_board.SetState<kOpenPosition>(check_move);
      }
      {
      bit_board.SetState<kWhiteStone>(check_move);

      const bool check_forbidden = bit_board.IsForbiddenMove<kBlackTurn>(move);
      
      if(check_forbidden != is_forbidden){
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

    const auto exact_label = is_forbidden ? kExactCheckDoubleThree : kExactCheckNonDoubleThree;
    const auto calc_label = is_forbidden ? kCalcCheckDoubleThree : kCalcCheckNonDoubleThree;

    (*check_result)[exact_label] += exact_influence_area.count();
    (*check_result)[calc_label] += influence_area.count();

    // 差分領域のチェック
    const auto diff_area = influence_area ^ exact_influence_area;

    if(!is_forbidden && diff_area.count() >= 7){
      MoveList diff_move;
      GetMoveList(diff_area, &diff_move);
      cout << "[Diff]board: " << board_sequence.str() << " , Move: " << MoveString(move) << ", ";
      cout << "diff: " << diff_move.str() << endl;
    }
  }
}

void GetTerminateGuardTest(const realcore::StringVector &record_string_list)
{
  map<string, SearchCounter> check_result;
  set<HashValue> checked_board;
  const auto record_count = record_string_list.size();

  for(size_t i=0; i<record_count; ++i){
    const auto& board_string = record_string_list[i];
    MoveList board_move_sequence(board_string);
    MoveList board_move;

    for(const auto move : board_move_sequence){
      const auto hash_value = CalcHashValue(board_move);

      if(checked_board.find(hash_value) == checked_board.end()){
        GetTerminateGuardCheck(board_move, &check_result);
        checked_board.insert(hash_value);
      }

      board_move += move;
    }

    if(i % 1000 == 0){
      cerr << i << endl;
    }
  }

  // Board::GetTerminateGuardの影響領域チェック結果
  cerr << "GetTerminateGuard:" << endl;
  cerr << "  Exact: " << check_result[kExactCheckTerminateGuard] << endl;
  cerr << "  Calc: " << check_result[kCalcCheckTerminateGuard] << endl;
  cerr << "  Calc/Exact: " << 1.0 * check_result[kCalcCheckTerminateGuard] / check_result[kExactCheckTerminateGuard] << endl;
}

void GetTerminateGuardCheck(const realcore::MoveList &board_sequence, std::map<std::string, realcore::SearchCounter> *check_result)
{
  const bool is_black_turn = board_sequence.IsBlackTurn();
  Board board(board_sequence);

  MovePosition terminating_move;
  
  if(board.TerminateCheck(is_black_turn, &terminating_move)){
    return;
  }

  const bool is_opponent_four = board.IsOpponentFour(nullptr);

  if(is_opponent_four){
    return;
  }

  // Passをした際に相手に終端手があるかをチェック
  board.MakeMove(kNullMove);
  const bool is_terminate_threat = board.TerminateCheck(!is_black_turn, &terminating_move);
  board.UndoMove();
  
  MoveBitSet guard_move_bit;
  const bool is_terminate_guard = board.GetTerminateGuard(&guard_move_bit);

  if(!is_terminate_threat){
    // 相手に１手勝ちがないが、１手勝ち防手を生成しているケース
    if(is_terminate_guard){
      MoveList guard_move;
      GetMoveList(guard_move_bit, &guard_move);

      cout << "[Guard move on non-threat]board: " << board_sequence.str() << " , guard: " << guard_move.str() << endl;
    }

    return;
  }

  MoveBitSet forbidden_bit;
  board.EnumerateForbiddenMoves(&forbidden_bit);

  MoveList candidate_move;
  board_sequence.GetOpenMove(forbidden_bit, &candidate_move);

  // 相手に１手勝ちがあるケース
  MoveBitSet exact_guard_bit;

  for(const auto move : candidate_move){
    board.MakeMove(move);

    MovePosition check_terminating_move;
    const bool check_terminate_threat = board.TerminateCheck(!is_black_turn, &check_terminating_move);
    
    if(!check_terminate_threat){
      exact_guard_bit.set(move);
    }

    board.UndoMove();
  }

  // 算出した防手集合が正確な防手集合を包含していることをチェック
  const bool is_include = ((guard_move_bit | exact_guard_bit) == guard_move_bit);

  if(!is_include){
    MoveList guard_move, exact_guard_move;
    GetMoveList(guard_move_bit, &guard_move);
    GetMoveList(exact_guard_bit, &exact_guard_move);
    
    cout << "[Not Include]board: " << board_sequence.str() << " , ";
    cout << "exact: " << exact_guard_move.str() << " , calc: " << guard_move.str() << endl;
  }

  (*check_result)[kExactCheckTerminateGuard] += exact_guard_bit.count();
  (*check_result)[kCalcCheckTerminateGuard] += guard_move_bit.count();

  // 差分領域のチェック
  const auto diff_area = guard_move_bit ^ exact_guard_bit;

  if(diff_area.count() >= 5){
    MoveList diff_move;
    GetMoveList(diff_area, &diff_move);
    cout << "[Diff]board: " << board_sequence.str() << " , ";
    cout << "diff: " << diff_move.str() << endl;
  }
}
