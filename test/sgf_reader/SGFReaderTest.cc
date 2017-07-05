#include "gtest/gtest.h"

#include "SGFReader.h"

using namespace std;

namespace realcore
{

class SGFReaderTest
: public ::testing::Test
{
public:
  SGFReaderTest(){
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
    sgf_reader_.ParseSGF(sgf_data_);
  }

  void GetGameDateTest(){
    const auto actual = sgf_reader_.GetGameDate();
    EXPECT_EQ("2003-10-07", actual);
  }

  void GetBlackPlayerNameTest()
  {
    const auto actual = sgf_reader_.GetBlackPlayerName();
    EXPECT_EQ("PlayerB", actual);
  }

  void GetBlackPlayerRankTest()
  {
    const auto actual = sgf_reader_.GetBlackPlayerRank();
    EXPECT_EQ("1d ", actual);
  }

  void GetWhitePlayerNameTest()
  {
    const auto actual = sgf_reader_.GetWhitePlayerName();
    EXPECT_EQ("PlayerA", actual);
  }

  void GetWhitePlayerRankTest()
  {
    const auto actual = sgf_reader_.GetWhitePlayerRank();
    EXPECT_EQ("3d*", actual);
  }

  void GetGameRuleTest()
  {
    const auto actual = sgf_reader_.GetGameRule();
    EXPECT_EQ("RIF", actual);
  }

  void GetGameEndStatusTest()
  {
    const auto actual = sgf_reader_.GetEndStatus();
    EXPECT_EQ(kResign, actual);
  }

  void GetGameResultTest()
  {
    const auto actual = sgf_reader_.GetGameResult();
    EXPECT_EQ(kWhiteWin, actual);
  }

  void GetGameRecordTest()
  {
    const auto actual = sgf_reader_.GetGameRecord();
    const string expect_record = "hhhgjfigiekgjgjhkigg";

    EXPECT_EQ(expect_record, actual);
  }

  void GetAlternativeMovesTest()
  {
    const auto actual = sgf_reader_.GetAlternativeMoves();
    const string expect_alternative = "gg; hh";

    EXPECT_EQ(expect_alternative, actual);
  }

  void GetEventNameTest()
  {
    const auto actual = sgf_reader_.GetEventName();
    EXPECT_EQ("none", actual);
  }
private:
  string sgf_data_;
  SGFReader sgf_reader_;
};

TEST_F(SGFReaderTest, GetGameDateTest)
{
  GetGameDateTest();
}

TEST_F(SGFReaderTest, GetBlackPlayerNameTest)
{
  GetBlackPlayerNameTest();
}

TEST_F(SGFReaderTest, GetBlackPlayerRankTest)
{
  GetBlackPlayerRankTest();
}

TEST_F(SGFReaderTest, GetWhitePlayerNameTest)
{
  GetWhitePlayerNameTest();
}

TEST_F(SGFReaderTest, GetWhitePlayerRankTest)
{
  GetWhitePlayerRankTest();
}

TEST_F(SGFReaderTest, GetGameRuleTest)
{
  GetGameRuleTest();
}

TEST_F(SGFReaderTest, GetGameEndStatusTest)
{
  GetGameEndStatusTest();
}

TEST_F(SGFReaderTest, GetGameResultTest)
{
  GetGameResultTest();
}

TEST_F(SGFReaderTest, GetGameRecordTest)
{
  GetGameRecordTest();
}

TEST_F(SGFReaderTest, GetAlternativeMovesTest)
{
  GetAlternativeMovesTest();
}

TEST_F(SGFReaderTest, GetEventNameTest)
{
  GetEventNameTest();
}
}   // namespace realcore
