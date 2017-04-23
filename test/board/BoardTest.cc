// @brief 非メンバ関数のテスト
#include <algorithm>
#include <array>

#include "gtest/gtest.h"

#include "Move.h"
#include "MoveList.h"
#include "Board.h"

using namespace std;

namespace realcore
{

class BoardTest
: public ::testing::Test
{
public:

};

TEST_F(BoardTest, IsEqualTest)
{
  Board board_1, board_2;

  EXPECT_TRUE(IsEqual(board_1, board_1));
  EXPECT_TRUE(IsEqual(board_1, board_2));

  board_1.MakeMove(kMoveHH);
  EXPECT_TRUE(IsEqual(board_1, board_1));
  EXPECT_FALSE(IsEqual(board_1, board_2));
  
  board_2.MakeMove(kMoveHH);
  EXPECT_TRUE(IsEqual(board_1, board_2));
}

TEST_F(BoardTest, CopyTest)
{
  Board board_1, board_2;

  board_1.MakeMove(kMoveHH);
  EXPECT_FALSE(IsEqual(board_1, board_2));

  Copy(board_1, &board_2);
  EXPECT_TRUE(IsEqual(board_1, board_2));
}

TEST_F(BoardTest, MakeNonTerminateNormalSequenceTest)
{
  vector<string> test_sequence_list;
    
  test_sequence_list.emplace_back("gghhhgghhigiigffgffghfgjhjifjfjijggeefgkeghehkkgkfdfkhdggdhlgllfhdlg");
  test_sequence_list.emplace_back("gghhghgiighgffhigjfghjgfifhfjijfhejgkfefdfegdggegdgkglhkhdkghlkhlglf");
  test_sequence_list.emplace_back("hhggghgihihgigfgffgjgfhfhjjfifjijgefeggehkgkkfhekhkggddfgldghdhllglf");
  test_sequence_list.emplace_back("giihhiiiffhffhhjgfeeifgeijhejfiejgjejhjkefkgfedeikdfkelekflf");
  test_sequence_list.emplace_back("higiihgfiihfffiffhijhjefjffejghejhieeeikgejejkkfkekgdfdelfle");
  test_sequence_list.emplace_back("gghgigiiihfffggffhjghfjhifjjijehjfeijihkfeikgedikffdkggdgllfhllg");
  test_sequence_list.emplace_back("iihfffifgfijhjjgjfjijheefeefgkgehejehkkfiekgkedfkhfldegl");
  test_sequence_list.emplace_back("hfffiffjjfgfjjgjeehjefijejgeekgkfeiefkjehekfhkkgikkhjkkjkekkkilelglfljlhlkli");
  test_sequence_list.emplace_back("fjffhjgfifgjijhfeejfefjjejekfegkfkhegehkieikkfjekgjkkikekjkhkklflelglilhljlk");
  test_sequence_list.emplace_back("hihhighgiiihfjffgfgjijhfeehjejiffkjfgejjheefieekikfejegkkjhkdkjklekelfkfljkklkdjmjmk");
  test_sequence_list.emplace_back("hhihhgiihifjiggfffgjhfhjijifjjjfeeekeffeejfkgkgeieheikhkjejkkfkekjdjkkljdklklemjlfmk");
  test_sequence_list.emplace_back("hhighgiihifjihgjffhfgfjfhjeeiffeijfkjjheefhkejieekikgejegkjkkekjkfdjkkdkljlemjlfmklk");
  test_sequence_list.emplace_back("hhhihgigffihgfiihffjhjgjijifeejfejjjekeffkfegehegkhkieikjkjekekjkfdjkkdklelfljmjlkmk");
  test_sequence_list.emplace_back("gghgghffigfgihgfiihffhijifjgjfjhjjjiefegfehkgeiekfikkgjedgdfglfdidgdjdhllglf");
  test_sequence_list.emplace_back("igggihghiihgfgfffhhfgfijifjgjfjhjijjefegfeiegeikhkkfjedfkgdgfdgdglidhllfjdlg");
  test_sequence_list.emplace_back("ggghhgigihffiifhfghfgfijifjfjhjgefjiegjjgefehkjeiekgikdfkfdgfdgdjdgllfhllgid");
  test_sequence_list.emplace_back("ggghihhgfgighfiiifffijfhjfgfjjjgfejhgejihkefikegjeiedfkffdkggldgjdgdlfhllgid");
  test_sequence_list.emplace_back("gghhgiihhgfghifiiggjiihfffhjfjijgfjgifeijfejjihejjiehkikkfkikghlkjliillj");
  test_sequence_list.emplace_back("gghhgiighgihhiffiifgfigjfjhfgfifhjjiijjjjfeijgejiehekihkkjikhlkfilkgljli");
  test_sequence_list.emplace_back("hhgigghghiigihffiifgfigjfjjigfjjhfeihjejifheijhkjfiejgikkikfkjkghlliillj");
  test_sequence_list.emplace_back("hhgigghghiihigiififffjfggfgjhfifhjjiijjjjfeijgejhkheikiekfkgkjkiilhllilj");
  test_sequence_list.emplace_back("ggihhgfghihjigifiiijffjgfjjhgfjjgjekhffkjfgkejhkikkkjkdjkjljdklk");
  test_sequence_list.emplace_back("gghgighifgihgfiihfffhjfjijgjjgifjjjfejjhekgkfkhkkjikkkjkdkdjlklj");
  test_sequence_list.emplace_back("hhgghigiighgihiigfffgjfjjijfekjgfkjjkekfkjdlldellekd");
  test_sequence_list.emplace_back("gghhhggiiihigfiggjihjfffjgfjjijjekfkkekfelkjkddlleld");
  test_sequence_list.emplace_back("fghifjihhjiijhgjegjgeheefkejkfekdegkdhkgdjddfddgledklfed");
  test_sequence_list.emplace_back("hhihhgiihifjiggjffhjgfifhfijjjjfeeefejekfkfegkgehkheiejeikjkkfkekkkgdkkjledjlfljlkmemjmk");
  test_sequence_list.emplace_back("hhighgihhiffiigffjhfgjhjifijefjffkjjgeeeheejieekikfekegkkfhkkjjekkjkdjkgledkljlfmelkmkmj");
  test_sequence_list.emplace_back("hhhihgigihiigjffhffjhjgfifeeijfkjfhejjhkefieejikekjefejkgekfgkkjkekkkgdjlfdkljlelkmemjmk");
  test_sequence_list.emplace_back("hhhgihhiiiiggjffhffjhjgfifijefjfejjjfeeegkekhefkiegejkhkkeikkgjedkkflekjlfkklkdjmjljmkme");
  test_sequence_list.emplace_back("hihhighgihffiihffjhjgfijgjefifekjffejjfkeegkejiegejehejkhkkeikkjkfkkkgdjdklflemeljmjlkmk");
  test_sequence_list.emplace_back("ggighgfffgfhfigfhjgjijhfjfifjgeejjegefeheifkejgkfeikhkjkkgkfkkkjfdedlglfljlk");
  test_sequence_list.emplace_back("hhgggighhihgihiggffghffhjggjjhhjegeeehgefeiegkjkhekgikdhjefdkhgdkkhldgiledjdhdklidlhjllklgll");
  test_sequence_list.emplace_back("hhgggighihhgfghifhighfgfhjgjehjggejhgkeeheegjefejkiekgikkhkkdgdhedfdgdhdidhljlilkljdlhlglkll");
  test_sequence_list.emplace_back("hhgghgghihgifghifhiggjgfhfeehjegjghejhikehjefekggekhgkdhiefdjkgdkkhddghledilidjdjlkllglklhll");
  test_sequence_list.emplace_back("hhggghgihihgigffgffggjifhfjihjgejfgkjgikefjkegkfhekghkkkkhdfhddgjlgdklhllgillklfmfll");
  test_sequence_list.emplace_back("hhggghgigfhggjhihfigjgffjifgeghjgeifikjfkgefkhgkkkhedfhkdgjkgdkfhlhdiljlkllflklgmfll");
  test_sequence_list.emplace_back("hhghgghggiighifgffgjgfhfhjjiifgkjfikjgjkefkgegkhgedfhedghkgdkfhdkkhljlillgkllklfmfll");
  test_sequence_list.emplace_back("hhgggighhgighifgffhfgfifgjjghjjijfegefhkgeikgkkghedfjkhdkfhlkhilkkkldglfgdlkjllllgmf");
  test_sequence_list.emplace_back("hhgghihgihigiifffggjgfhjhfjfifjggkefieegkffkkghedfdgelflhdidlglf");
  test_sequence_list.emplace_back("hhgiiiihfhfggjgfjgifjhejjjjkegkhehfleijdielficll");
  test_sequence_list.emplace_back("hhffgiifhiijfjjihfekhjkgejdifedlhehdikjljekl");
  test_sequence_list.emplace_back("gghhghiggiiihigjihijfgjffjeejgekjjfegefkhejkiekejekfkkdleledgdfdkdhdldidlejd");
  test_sequence_list.emplace_back("ggghgihgihigfgfffhgjfijhfjjigffegefkgkflkigdfdgl");
  test_sequence_list.emplace_back("hhggiighfggifjhigjigjgiheeijgejfhejjieekjefejkfkkkkeedkfeldlhdfdkdgdldidlejd");
  test_sequence_list.emplace_back("gghhghihhgfgigfhijjfjhjgjjjiegeefeehhegehkikiejekekikfedkggdkhglfdhdhlidjdkd");
  test_sequence_list.emplace_back("hhggffghfggigjhgifhijfigjgfjefgfekhfgeegkgfkkhhekikfkjkkdlgdellfhdlhjllikllklgimljjm");
  test_sequence_list.emplace_back("hhhiggihhgffiifggfhjgjjgegjhehfkgegkjehekeiekgkfhddhideljdfllekddmldemlf");
  test_sequence_list.emplace_back("fggghjiiijffjhefjigkeekehkkgkfdekhglfllflelg");
  test_sequence_list.emplace_back("gghhghhgighiihffiififggffjhfhjifjfijefjgegekejkfhekghkdfiedgikdkhlhdidillglf");
  test_sequence_list.emplace_back("fghigfihhfiiifffjfgjjghjjhehegfkgehegkiekejekgkfdhflelhdlelfdmem");
  test_sequence_list.emplace_back("hhhighiigigfffifgjjfhfjhhjeejifeefgefkgkiehejekekfkgkhdfdeelfllelflg");
  test_sequence_list.emplace_back("hhihgiijfheffifkfjkfjfdfjidjehlgkecjlfdmimjn");
  test_sequence_list.emplace_back("hhfigggjghjfffjifheejhikeikefeddfkdikfdjkhgdkjlhcklk");
  test_sequence_list.emplace_back("hhggihgifghgfhigfjjgeijikgfkkhkkkjdkdfdllicgciclckmkmfmlningnknl");
  test_sequence_list.emplace_back("hhfgggjhhgjjiheffheihfheieedkiflkkjdddlfhdljlkcglmfcmhkmmnmkngbh");
  test_sequence_list.emplace_back("hhggihfigjifjijfefjgfejjgkeihegehkjkkfdfkgelkkildekldhleedlfglcehdcgjdclljdmllfcchgmcikmhcmimcbcmkdnbkhbenibfnlngbmbkbncnbnlndagnkah");
  test_sequence_list.emplace_back("hhgighfffghffhhjfiijfjefgffkjfkgjgdfjidkjjkleicfejemgkfmhekcjemijkmlkfbbhlbdjdbjjlbnkdebcgfncimbcmngbcadbfafbihobkiojbkajnlalnmaaeocaiodcookjaoljo");
  test_sequence_list.emplace_back("hhbbgibdiibefhbffjbhhfbiifbmijbnjhcbeednefebejfnfegbgkhnikibkfinkjkbkklndgmbdhnbdincfdndidnfjdngcenicknjclnlfmnmhmnnjmaameacmhadmiaemjafbcahbgaibjajbkamblancnaodbcaencofbdagndohbeajbeojnfaknfolbhamnianeionhkankkoablaagmaakmoalnobaoaboobgaocgoodhoogjaohjooiloojnaoloeomofonokoo");
  test_sequence_list.emplace_back("hhhfghefhgehigejegheeikefkdegkdkjegdkihlkjjldglgdichelckildmjdfmlljcimjmlclmmgmemhmfmjmkbkbibnengnndhnnfjnnjknaglngoalnoofok");
  test_sequence_list.emplace_back("hhiifhfgfigfjfefjgejeifkjkgkdfgddgiddkilfdliflcehlcfjdckjlgclfgmljhmcdimchjmcjlcdmlmecmdfmmgjcmhkmmimebdmkbfmlbgbcblcbbmgbdbgndninhnjbkbknlnnhnbninfnnnjaenkanaafoamhobojacomadanodoodjooekooimookocoo");
  test_sequence_list.emplace_back("hhggihghfgfhifekjffeeeheefkfeikgejelkhfldjhdedlfjdljckechcemmhmebgmkblbhdnbjnddbnehbnghnnhahniaknjamnldanmfoboohcaokco");
  test_sequence_list.emplace_back("hhgigghgghhifgigfjffgfgjhfjfifefjgekeghefkkfgekhkgkkkidlkjgdelhdjllgkllhlfljlilkimjm");
  test_sequence_list.emplace_back("hhfjghfkfhdgjhdheh");
  test_sequence_list.emplace_back("hhhgggifgheeffeifhgkhfheijkijhldfklghkchkeemkhgmhdmijlbifmhbjcnf");
  test_sequence_list.emplace_back("hhfghgeghihehfiehk");
  test_sequence_list.emplace_back("hhfigifjhijiigejihhehfiejhkhjdki");
  test_sequence_list.emplace_back("hhgggighighiihgjiiijfgjffjjgjjeefeekfkgejehejkiekekkkfdledelgdfdhdidjdkdleld");
  test_sequence_list.emplace_back("hhfjggifhijfihjjfhehhfeikehemeje");

  for(const auto &sequence_str : test_sequence_list){
    MoveList move_list(sequence_str);
    ASSERT_FALSE(IsNonTerminateNormalSequence(move_list));

    MoveList modified_list;
    const auto is_modified = MakeNonTerminateNormalSequence(move_list, &modified_list);
    ASSERT_TRUE(is_modified);

    BitBoard before_board(move_list), after_board(modified_list);
    ASSERT_TRUE(before_board == after_board);

    ASSERT_TRUE(IsNonTerminateNormalSequence(modified_list));
  }
}

}   // namespace realcore
