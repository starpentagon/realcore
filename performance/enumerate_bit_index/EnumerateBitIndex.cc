#include <iostream>
#include <random>
#include <chrono>
#include <array>

#include <boost/program_options.hpp>

#include "EnumerateBitIndex.h"

using namespace std;
using namespace boost;
using namespace boost::program_options;
using namespace realcore;

int main(int argc, char* argv[])
{
  // オプション設定
  options_description option;

  option.add_options()
    ("count,c", value<uint64_t>()->default_value(10000), "反復回数(default: 1万回)")
    ("help,h", "ヘルプを表示");
  
  variables_map arg_map;
  store(parse_command_line(argc, argv, option), arg_map);

  if(arg_map.count("help")){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << option;
    cout << endl;
    cout << "Repeat the following procedure on specified times * OnBitPattern(0-64):" << endl;
    cout << " 1. Scanning the bit sequence method" << endl;
    cout << " 2. Table lookup method" << endl;
    cout << " 3. Enumerate rightmost bit index" << endl;
    cout << endl;

    return 0;
  }

  // 乱数初期化
  random_device seed_generator;
  size_t seed = seed_generator();
  mt19937_64 random_generator(seed);

  cout << "random seed:\t" << seed << endl;

  // OnBit数ごとに統計を取るためOnBit数別に乱数を記録する
  const uint64_t iteration_count = arg_map["count"].as<uint64_t>();
  constexpr size_t kBitPattern = 65;    // OnBit数: 0-64の65通り

  // 格納リストの初期化
  std::array<vector<uint64_t>, kBitPattern> on_bit_value_list;

  for(auto &value : on_bit_value_list){
    value.reserve(iteration_count);
  }

  // OnBit数ごとに乱数を生成する
  for(size_t on_bit_count=0; on_bit_count<kBitPattern; ++on_bit_count){
    for(size_t i=0; i<iteration_count; i++){
      std::array<size_t, 64> shift_value_map{{0}};   // 生成済みのシフト値
      size_t shift_count = 0;
      uint64_t value = 0;

      while(shift_count != on_bit_count){
        size_t shift = random_generator() % 64;
        
        if(shift_value_map[shift] == 0){
          shift_value_map[shift] = 1;
          ++shift_count;

          value |= 1ULL << shift;
        }
      }

      on_bit_value_list[on_bit_count].push_back(value);
    }
  }

  // ScanBitSequence
  vector<uint64_t> time_scan;
  time_scan.reserve(kBitPattern);

  {
    uint64_t index_sum = 0;
    vector<size_t> index_list;
    index_list.reserve(64);

    for(const auto &value_list : on_bit_value_list){
      auto start_time = chrono::system_clock::now();
  
      for(auto value : value_list){
        index_list.clear();
        ScanBitSequence(value, &index_list);
        index_sum = accumulate(index_list.begin(), index_list.end(), index_sum);
      }

      auto elapsed_time = chrono::system_clock::now() - start_time;
      time_scan.push_back(chrono::duration_cast<chrono::milliseconds>(elapsed_time).count());
    }

    cout << "ScanBitSequence index sum: " << index_sum << endl;
  }

  // TableLookup
  vector<uint64_t> time_shift_map;
  time_shift_map.reserve(kBitPattern);
  
  {
    uint64_t index_sum = 0;
    vector<size_t> index_list;
    index_list.reserve(64);

    for(const auto &value_list : on_bit_value_list){
      auto start_time = chrono::system_clock::now();
  
      for(auto value : value_list){
        index_list.clear();
        ShiftMap(value, &index_list);
        index_sum = accumulate(index_list.begin(), index_list.end(), index_sum);
      }
  
      auto elapsed_time = chrono::system_clock::now() - start_time;
      time_shift_map.push_back(chrono::duration_cast<chrono::milliseconds>(elapsed_time).count());
    }

    cout << "TableLookup index sum: " << index_sum << endl;
  }

  // EnumerateRightmostBit
  vector<uint64_t> time_rightmost;
  time_rightmost.reserve(kBitPattern);

  {
    uint64_t index_sum = 0;
    vector<size_t> index_list;
    index_list.reserve(64);

    for(const auto &value_list : on_bit_value_list){
      auto start_time = chrono::system_clock::now();
  
      for(auto value : value_list){
        index_list.clear();
        EnumerateRightmostBit(value, &index_list);
        index_sum = accumulate(index_list.begin(), index_list.end(), index_sum);
      }
  
      auto elapsed_time = chrono::system_clock::now() - start_time;
      time_rightmost.push_back(chrono::duration_cast<chrono::milliseconds>(elapsed_time).count());
    }

    cout << "EnumerateRightmostBit index sum: " << index_sum << endl;
  }

  // 経過時間を出力
  cout << "PopCount,ScanBitSequence(ms),TableLookup(ms),EnumerateRightmostBit(ms)" << endl;

  for(size_t i=0; i<kBitPattern; i++){
    cout << i << ",";
    cout << time_scan[i] << ",";
    cout << time_shift_map[i] << ",";
    cout << time_rightmost[i] << endl;
  }

  return 0;
}
