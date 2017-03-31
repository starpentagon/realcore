#include "gtest/gtest.h"

#include "CSVReader.h"

using namespace std;
using namespace realcore;

TEST(CSVReaderTest, CSVSplitterTest)
{
  const string test_str = "aa,bb,,cc";
  StringVector string_list;

  CSVSplitter(test_str, &string_list);

  ASSERT_EQ(4, string_list.size());

  EXPECT_EQ("aa", string_list[0]);
  EXPECT_EQ("bb", string_list[1]);
  EXPECT_EQ("", string_list[2]);
  EXPECT_EQ("cc", string_list[3]);
}

TEST(CSVReaderTest, ReadCSVTest)
{
  // test_data.csv:
  // col1,col2,col3
  // a,,c
  // ,,
  // 1,2,3
  map<string, StringVector> csv_data;
  const bool is_read = ReadCSV("test_data.csv", &csv_data);
  ASSERT_TRUE(is_read);

  {
    // col1
    const auto &string_list = csv_data["col1"];
    
    ASSERT_EQ(3, string_list.size());

    EXPECT_EQ("a", string_list[0]);
    EXPECT_EQ("", string_list[1]);
    EXPECT_EQ("1", string_list[2]);
  }
  {
    // col2
    const auto &string_list = csv_data["col2"];
    
    ASSERT_EQ(3, string_list.size());

    EXPECT_EQ("", string_list[0]);
    EXPECT_EQ("", string_list[1]);
    EXPECT_EQ("2", string_list[2]);
  }
  {
    // col3
    const auto &string_list = csv_data["col3"];
    
    ASSERT_EQ(3, string_list.size());

    EXPECT_EQ("c", string_list[0]);
    EXPECT_EQ("", string_list[1]);
    EXPECT_EQ("3", string_list[2]);
  }
}
