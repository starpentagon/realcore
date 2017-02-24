#include <iostream>
#include <cstdint>
#include <random>
#include <chrono>

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
    ("count,c", value<uint64_t>()->default_value(1000 * 10000), "反復回数(default: 1000万回)")
    ("help,h", "ヘルプを表示");
  
  variables_map arg_map;
  store(parse_command_line(argc, argv, option), arg_map);

  if(arg_map.count("help")){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << option;
    cout << endl;
    cout << "Repeat the following procedure on specified times:" << endl;
    cout << " 1. Construct Board object" << endl;
    cout << " 2. SetState BlackStone/WhiteStone alternatively along with the shuffled list" << endl;
    cout << " 3. GetState along with the shuffled list" << endl;
    cout << " 4. SetState OpenPosition along with the shuffled list" << endl;
    cout << endl;
    cout << "Run SetState: 450 * n times, GetState: 225 * n times in total." << endl;

    return 0;
  }

  const uint64_t iteration_count = arg_map["count"].as<uint64_t>();
  auto in_board_move = GetAllInBoardMove();
  shuffle(in_board_move.begin(), in_board_move.end(), mt19937_64());

  auto start_time = chrono::system_clock::now();

  for(size_t i=0; i<iteration_count; i++)
  {
    Board board;
    PositionState state = kBlackStone;

    for(const auto move : in_board_move){
      board.SetState(move, state);
      
      state = board.GetState(move);
      state = static_cast<PositionState>(3 - state);
    }

    for(const auto move : in_board_move){
      board.SetState<kOpenPosition>(move);
    }
  }

  auto elapsed_time = chrono::system_clock::now() - start_time;
  
  cout << "elapsed time: ";
  cout << chrono::duration_cast<chrono::milliseconds>(elapsed_time).count();
  cout << " ms" << endl;

  return 0;
}