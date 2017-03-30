//! @file
//! @brief CSVファイルの読込クラス
//! @brief CSVのカラムごとの文字列リストを生成する
//! @author Koichi NABETANI
//! @date 2017/02/22
#ifndef CSV_READER_H
#define CSV_READER_H

#include <vector>
#include <map>
#include <string>

namespace realcore{

//! @brief 文字列リスト
typedef std::vector<std::string> StringVector;

//! @brief Header付きのCSVを読み込む関数
//! @param csv_file CSVファイルパス
//! @param *csv_data 読み込んだCSVデータ(headerをkeyとするStringVector)
//! @retval true 読み込みに成功
bool ReadCSV(const std::string &csv_file, std::map<std::string, StringVector> * const csv_data);

//! 1行のテキストデータをカンマで区切る関数
void CSVSplitter(const std::string &line, StringVector * const line_data);

}   // realcore

#endif    // CSV_READER_H