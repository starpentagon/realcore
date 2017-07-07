#include "gtest/gtest.h"

#include "MoveList.h"
#include "SGFParser.h"

using namespace std;

namespace realcore
{

class SGFParserTest
: public ::testing::Test
{
public:
  SGFParserTest()
  : sgf_parser_(kSGFCheckAll)
  {
    sgf_data_ = ""
"(;"
"GM[5]FF[4]"
"SW[NO]"
"5A[gg][hh]"
"GN[PlayerA-PlayerB(B) : RIF rule]"
"EV[none]"
"RE[W+Resign]"
"PW[PlayerA]WR[3d*]"
"PB[PlayerB]BR[1d ]"
"DT[2003-10-07]"
"SZ[15]TM[600]KM[5.5]"
""
";B[hh];W[hg];B[jf];W[ig]C["
" A started observation."
"]"
";B[ie];W[kg];B[jg];W[jh];B[ki];W[gg]"
";)"
""
"---";
    sgf_parser_.ParseSGF(sgf_data_);
  }

  void GetGameDateTest(){
    const auto actual = sgf_parser_.GetGameDate();
    EXPECT_EQ("2003-10-07", actual);
  }

  void GetBlackPlayerNameTest()
  {
    const auto actual = sgf_parser_.GetBlackPlayerName();
    EXPECT_EQ("PlayerB", actual);
  }

  void GetBlackPlayerRankTest()
  {
    const auto actual = sgf_parser_.GetBlackPlayerRank();
    EXPECT_EQ("1d ", actual);
  }

  void GetWhitePlayerNameTest()
  {
    const auto actual = sgf_parser_.GetWhitePlayerName();
    EXPECT_EQ("PlayerA", actual);
  }

  void GetWhitePlayerRankTest()
  {
    const auto actual = sgf_parser_.GetWhitePlayerRank();
    EXPECT_EQ("3d*", actual);
  }

  void GetGameRuleTest()
  {
    const auto actual = sgf_parser_.GetGameRule();
    EXPECT_EQ("RIF", actual);
  }

  void GetGameEndStatusTest()
  {
    const auto actual = sgf_parser_.GetEndStatus();
    EXPECT_EQ(kResign, actual);
  }

  void GetGameResultTest()
  {
    const auto actual = sgf_parser_.GetGameResult();
    EXPECT_EQ(kWhiteWin, actual);
  }

  void GetGameRecordTest()
  {
    const auto actual = sgf_parser_.GetGameRecord();
    const string expect_record = "hhhgjfigiekgjgjhkigg";

    EXPECT_EQ(expect_record, actual);
  }

  void GetAlternativeMovesTest()
  {
    const auto actual = sgf_parser_.GetAlternativeMoves();
    const string expect_alternative = "gg; hh";

    EXPECT_EQ(expect_alternative, actual);
  }

  void GetEventNameTest()
  {
    const auto actual = sgf_parser_.GetEventName();
    EXPECT_EQ("none", actual);
  }

  void GetMoveListFromSGFDataTest()
  {
      MoveList actual_list;
      GetMoveListFromSGFData(kSGFCheckNone, sgf_data_, &actual_list);

      MoveList expected_list("hhhgjfigiekgjgjhkigg");
      EXPECT_EQ(expected_list, actual_list);      
  }

private:
  string sgf_data_;
  SGFParser sgf_parser_;
};

TEST_F(SGFParserTest, GetGameDateTest)
{
  GetGameDateTest();
}

TEST_F(SGFParserTest, GetBlackPlayerNameTest)
{
  GetBlackPlayerNameTest();
}

TEST_F(SGFParserTest, GetBlackPlayerRankTest)
{
  GetBlackPlayerRankTest();
}

TEST_F(SGFParserTest, GetWhitePlayerNameTest)
{
  GetWhitePlayerNameTest();
}

TEST_F(SGFParserTest, GetWhitePlayerRankTest)
{
  GetWhitePlayerRankTest();
}

TEST_F(SGFParserTest, GetGameRuleTest)
{
  GetGameRuleTest();
}

TEST_F(SGFParserTest, GetGameEndStatusTest)
{
  GetGameEndStatusTest();
}

TEST_F(SGFParserTest, GetGameResultTest)
{
  GetGameResultTest();
}

TEST_F(SGFParserTest, GetGameRecordTest)
{
  GetGameRecordTest();
}

TEST_F(SGFParserTest, GetAlternativeMovesTest)
{
  GetAlternativeMovesTest();
}

TEST_F(SGFParserTest, GetEventNameTest)
{
  GetEventNameTest();
}

TEST_F(SGFParserTest, GetMoveListFromSGFDataTest)
{
  GetMoveListFromSGFDataTest();
}

TEST_F(SGFParserTest, GetMoveListFromSGFBoardTest)
{
  {
    // 配石数一致, 黒番
    const string sgf_data = "(;GM[4]FF[4]CA[UTF-8]AP[Web Renju Board:1.0.0]ST[0]SZ[15]PB[Black]PW[White]AB[hh][ig]AW[gi][gh])";
    MoveList actual_list;
    GetMoveListFromSGFData(kSGFCheckNone, sgf_data, &actual_list);

    MoveList expect_list("hhgiiggh");
    EXPECT_EQ(expect_list, actual_list);
  }
  {
    // 配石数一致, 白番
    const string sgf_data = "(;GM[4]FF[4]CA[UTF-8]AP[Web Renju Board:1.0.0]ST[0]SZ[15]PB[Black]PW[White]AB[hh][ig][gg]AW[gi][gh])";
    MoveList actual_list;
    GetMoveListFromSGFData(kSGFCheckNone, sgf_data, &actual_list);

    MoveList expect_list("hhgiigghgg");
    EXPECT_EQ(expect_list, actual_list);
  }
  {
    // 配石数一致, 白番指定
    const string sgf_data = "(;GM[4]FF[4]CA[UTF-8]AP[Web Renju Board:1.0.0]PL[W]ST[0]SZ[15]PB[Black]PW[White]AB[hh][ig]AW[gi][gh])";
    MoveList actual_list;
    GetMoveListFromSGFData(kSGFCheckNone, sgf_data, &actual_list);

    MoveList expect_list("hhgiigghpp");
    EXPECT_EQ(expect_list, actual_list);
  }
  {
    // 配石数一致, 黒番指定
    const string sgf_data = "(;GM[4]FF[4]CA[UTF-8]AP[Web Renju Board:1.0.0]PL[B]ST[0]SZ[15]PB[Black]PW[White]AB[hh][ig][gg]AW[gi][gh])";
    MoveList actual_list;
    GetMoveListFromSGFData(kSGFCheckNone, sgf_data, &actual_list);

    MoveList expect_list("hhgiigghggpp");
    EXPECT_EQ(expect_list, actual_list);
  }
  {
    // 配石数不一致, 手番指定なし
    const string sgf_data = "(;GM[4]FF[4]CA[UTF-8]AP[Web Renju Board:1.0.0]ST[0]SZ[15]PB[Black]PW[White]AB[hh][ig][jh]AW[ii])";
    MoveList actual_list;
    GetMoveListFromSGFData(kSGFCheckNone, sgf_data, &actual_list);

    MoveList expect_list("hhiiigppjhpp");
    EXPECT_EQ(expect_list, actual_list);
  }
  {
    // 配石数不一致, 手番指定あり
    const string sgf_data = "(;GM[4]FF[4]CA[UTF-8]AP[Web Renju Board:1.0.0]PL[W]ST[0]SZ[15]PB[Black]PW[White]AB[hh][ig][jh]AW[ii])";
    MoveList actual_list;
    GetMoveListFromSGFData(kSGFCheckNone, sgf_data, &actual_list);

    MoveList expect_list("hhiiigppjh");
    EXPECT_EQ(expect_list, actual_list);
  }
}
}   // namespace realcore
