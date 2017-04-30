#ifndef INFLUENCE_AREA_CHECK_H
#define INFLUENCE_AREA_CHECK_H

#include <string>
#include <map>

#include "MoveList.h"
#include "BitBoard.h"
#include "SearchManager.h"
#include "CSVReader.h"

static const std::string kCheckExactForbidden = "ExactForbidden";                   // 戻り値がkForbiddenMove時の正確な影響領域
static const std::string kCheckExactPossibleForbidden = "ExactPossibleForbidden";   // 戻り値がkPossibleForbiddenMove時の正確な影響領域
static const std::string kCheckExactNonForbidden = "ExactNonForbidden";             // 戻り値がkNonForbiddenMove時の正確な影響領域

static const std::string kCheckCalcForbidden = "CalcForbidden";                   // 戻り値がkForbiddenMove時の算出した影響領域
static const std::string kCheckCalcPossibleForbidden = "CalcPossibleForbidden";   // 戻り値がkPossibleForbiddenMove時の算出した影響領域
static const std::string kCheckCalcNonForbidden = "CalcNonForbidden";             // 戻り値がkNonForbiddenMove時の算出した影響領域

void LineNeighborhoodForbiddenCheck(const realcore::MoveList &board_sequence, std::map<std::string, realcore::SearchCounter> *check_result);
void LineNeighborhoodForbiddenTest(const realcore::StringVector &record_string_list);

static const std::string kExactCheckDoubleThree = "ExactCheckDoubleThree";
static const std::string kExactCheckNonDoubleThree = "ExactCheckNonDoubleThree";
static const std::string kCalcCheckDoubleThree = "CalcCheckDoubleThree";
static const std::string kCalcCheckNonDoubleThree = "CalcCheckNonDoubleThree";

void DoubleThreeCheck(const realcore::MoveList &board_sequence, std::map<std::string, realcore::SearchCounter> *check_result);
void DoubleThreeTest(const realcore::StringVector &record_string_list);

#endif
