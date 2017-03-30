#include <cassert>
#include <fstream>
#include <iostream>

#include <boost/tokenizer.hpp>

#include "CSVReader.h"

using namespace std;
using namespace boost;

namespace realcore
{
bool ReadCSV(const std::string &csv_file, std::map<std::string, StringVector> * const csv_data)
{
  assert(csv_data != nullptr);
  assert(csv_data->empty());
  
  // ヘッダリストと行数を取得する
  size_t line_num = 0;
  StringVector header;

  {
    ifstream file_data(csv_file.c_str());

    if(file_data.fail()){
      cerr << "Failed to open the file: " << csv_file << endl;
      return false;
    }

    while(!file_data.eof())
    {
      string line;
      getline(file_data, line);

      if(line_num == 0){
        // header
        CSVSplitter(line, &header);
        line_num++;
        continue;
      }

      if(line.size() == 0){
        // 空行はスキップする
        continue;
      }

      line_num++;
    }
  }

  // リストサイズを初期化
  for(const auto col : header){
    (*csv_data)[col].reserve(line_num);
  }

  // CSVデータを読み込む
  ifstream file_data(csv_file.c_str());
  bool is_header = true;

  while(!file_data.eof())
  {
    string line;
    getline(file_data, line);

    if(is_header){
      is_header = false;
      continue;
    }

    if(line.size() == 0){
      // 空行はスキップする
      continue;
    }

    StringVector line_data;
    CSVSplitter(line, &line_data);

    if(header.size() != line_data.size()){
      // ヘッダとデータ行の列数が異なる
      cerr << "Column numbers are not same." << endl;
      cerr << "\theader num: " << header.size() << endl;
      cerr << "\tline num: " << line_data.size() << endl;
      cerr << "\tline data: " << line << endl;
      return false;
    }

    for(size_t i=0, size=line_data.size(); i<size; i++){
      const auto col = header[i];
      (*csv_data)[col].push_back(line_data[i]);
    }
  }

  return true;
}

void CSVSplitter(const std::string &line, StringVector * const line_data)
{
  assert(line_data != nullptr);
  assert(line_data->empty());
  
  typedef char_separator<char> char_sep;
  char_sep sep(",", "", keep_empty_tokens);

  tokenizer<char_sep> tokens(line, sep);
  tokenizer<char_sep>::iterator it = tokens.begin(), it_end = tokens.end();

  size_t size = distance(it, it_end);
  line_data->reserve(size);

  for(; it!=it_end; ++it){
    line_data->push_back(*it);
  }
}

}

