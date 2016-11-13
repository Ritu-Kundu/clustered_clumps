#include <limits.h>
#include <vector>
#include <string>
#include <iterator>
#include "Parser.hpp"
#include "Search.hpp"
#include "globalDefs.hpp"
#include "gtest/gtest.h"

using namespace cluclu;

TEST(searchTest, SearchTypeATest) {
  DEG_PATTERNS_DS dpds;
  Parser::parse_degenerate_patterns("test_files/samplePatternDeg.txt",dpds);
  TEXT t;
  Parser::parse_simple_text("test_files/sampleTextSolid.txt", dpds.alphabet, t);

  CLUSTERED_CLUMPS result = Search::find_TypeA(dpds, t);
  CLUSTERED_CLUMPS answer = {
    {4, 23}
  };

  for (int i=0; i < answer.size(); ++i) {
    EXPECT_EQ(result[i].first, answer[i].first);
    EXPECT_EQ(result[i].second, answer[i].second);
  } 

}

TEST(searchTest, SearchTypeBTest) {
  DEG_PATTERNS_DS dpds;

  Parser::parse_degenerate_patterns("test_files/samplePatternSolid.txt",dpds);

  DEG_TEXT_DS dtds;
  Parser::parse_degenerate_text("test_files/sampleTextDeg.txt", dtds);

  CLUSTERED_CLUMPS result = Search::find_TypeB(dpds.patterns, dtds);
  CLUSTERED_CLUMPS answer = {
    {4, 9}
  };

  for (int i=0; i < answer.size(); ++i) {
    EXPECT_EQ(result[i].first, answer[i].first);
    EXPECT_EQ(result[i].second, answer[i].second);
  } 

}

TEST(searchTest, SearchTypeCTest) {
  DEG_PATTERNS_DS dpds;
  Parser::parse_degenerate_patterns("test_files/samplePatternDeg.txt",dpds);

  DEG_TEXT_DS dtds;
  Parser::parse_degenerate_text("test_files/sampleTextDegC.txt", dtds);

  CLUSTERED_CLUMPS result = Search::find_TypeC(dpds, dtds);
  CLUSTERED_CLUMPS answer = {
    {4, 14},
    {18, 10}
  };

  for (int i=0; i < answer.size(); ++i) {
    EXPECT_EQ(result[i].first, answer[i].first);
    EXPECT_EQ(result[i].second, answer[i].second);
  } 

}




