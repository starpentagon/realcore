#include "gtest/gtest.h"

#include "SGFParser.h"

using namespace std;

namespace realcore
{

class SGFParserTest
: public ::testing::Test
{
public:
  SGFParserTest(){
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
}   // namespace realcore
