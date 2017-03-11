#include <iostream>
#include <cstdint>
#include <random>
#include <chrono>
#include <array>

#include <boost/program_options.hpp>

#include "Move.h"
#include "Board.h"

using namespace std;
using namespace boost;
using namespace boost::program_options;
using namespace realcore;

int main(int argc, char* argv[])
{
  // オプション設定
  options_description option;

  option.add_options()
    ("count,c", value<uint64_t>()->default_value(100 * 10000), "反復回数(default: 100万回)")
    ("help,h", "ヘルプを表示");
  
  variables_map arg_map;
  store(parse_command_line(argc, argv, option), arg_map);

  if(arg_map.count("help")){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << option;
    cout << endl;
    cout << "Repeat the following procedure on specified times:" << endl;
    cout << " 1. Construct Board object" << endl;
    cout << " 2. SetState BlackStone along with the shuffled list" << endl;
    cout << " 3. SetState WhiteStone along with the shuffled list" << endl;
    cout << " 4. SetState BlackStone/WhiteStone alternately along with the shuffled list" << endl;
    cout << " 5. SetState OpenPosition along with the shuffled list" << endl;
    cout << " 6. GetState along with the shuffled list" << endl;
    cout << endl;

    return 0;
  }

  const uint64_t iteration_count = arg_map["count"].as<uint64_t>();
  auto in_board_move = GetAllInBoardMove();

  // SetState: kBlackStone
  shuffle(in_board_move.begin(), in_board_move.end(), mt19937_64());

  {
    auto start_time = chrono::system_clock::now();

    for(size_t i=0; i<iteration_count; i++)
    {
      Board board;

      for(const auto move : in_board_move){
        board.SetState<kBlackStone>(move);
      }
    }

    auto elapsed_time = chrono::system_clock::now() - start_time;
    
    cout << "SetState<kBlackStone>:\t";
    cout << chrono::duration_cast<chrono::milliseconds>(elapsed_time).count();
    cout << " ms" << endl;
  }
  {
    auto start_time = chrono::system_clock::now();

    for(size_t i=0; i<iteration_count; i++)
    {
      Board board;

      for(const auto move : in_board_move){
        board.SetState<kWhiteStone>(move);
      }
    }

    auto elapsed_time = chrono::system_clock::now() - start_time;
    
    cout << "SetState<kWhiteStone>:\t";
    cout << chrono::duration_cast<chrono::milliseconds>(elapsed_time).count();
    cout << " ms" << endl;
  }
  {
    auto start_time = chrono::system_clock::now();
    PositionState state = kBlackStone;

    for(size_t i=0; i<iteration_count; i++)
    {
      Board board;

      for(const auto move : in_board_move){
        board.SetState(move, state);
        state = static_cast<PositionState>(3 - state);
      }
    }

    auto elapsed_time = chrono::system_clock::now() - start_time;
    
    cout << "SetState(Alternately):\t";
    cout << chrono::duration_cast<chrono::milliseconds>(elapsed_time).count();
    cout << " ms" << endl;
  }
  {
    auto start_time = chrono::system_clock::now();

    for(size_t i=0; i<iteration_count; i++)
    {
      Board board;

      for(const auto move : in_board_move){
        board.SetState<kOpenPosition>(move);
      }
    }

    auto elapsed_time = chrono::system_clock::now() - start_time;
    
    cout << "SetState<kOpenPosition>:\t";
    cout << chrono::duration_cast<chrono::milliseconds>(elapsed_time).count();
    cout << " ms" << endl;
  }
  {
    // 状態取得のみだと最適化された際に不要な参照として削除されることがあるため簡単な集計＆結果表示を行う
    auto start_time = chrono::system_clock::now();
    std::array<uint64_t, 4> state_count{{0}};

    for(size_t i=0; i<iteration_count; i++)
    {
      Board board;

      for(const auto move : in_board_move){
        const auto state = board.GetState(move);
        ++state_count[state];
      }
    }

    auto elapsed_time = chrono::system_clock::now() - start_time;
    
    cout << "GetState:\t";
    cout << chrono::duration_cast<chrono::milliseconds>(elapsed_time).count();
    cout << " ms" << endl;

    cout << "Summary result: " << state_count[0] << "," << state_count[1] << "," << state_count[2] << "," << state_count[3] << endl;
  }

  return 0;
}