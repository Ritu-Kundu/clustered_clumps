#include <limits.h>
#include <vector>
#include <string>
#include <iterator>
#include "Parser.hpp"
#include "globalDefs.hpp"
#include "gtest/gtest.h"

using namespace cluclu;

TEST(parserTest, VariousDegeneratePatterns) {
  DEG_PATTERNS_DS dpds;
  Parser::parse_degenerate_patterns("test_files/testPattern.txt",dpds);

  ALPHABET alphabet = "abcd";
  PATTERNS p = {
    {0,0,1},
    {2,2},
    {3,0},    
    {0,0,1,2},
    {0,0,1},
    {0,0,1},
    {0,0,1},
    {1,1},
    {0,0,1}
  };

  DEG_SYMBOLS ds = {
    {1,0,1,0},
    {1,1,0,0},
    {1,1,1,1},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0}
    
  };
  DEG_SUB_PATTERN s00(0,{});
  DEG_SUB_PATTERN s01(1,{0});
  DEG_SUB_PATTERN s02(2,{1,2});

  DEG_SUB_PATTERN s10(-1,{3,4,5});

  DEG_SUB_PATTERN s20(3,{});

  DEG_SUB_PATTERN s30(4,{6,7});

  DEG_SUB_PATTERN s40(5,{});
  DEG_SUB_PATTERN s41(-1,{8,9});

  DEG_SUB_PATTERN s50(6,{});
  DEG_SUB_PATTERN s51(7,{10,11});

  DEG_SUB_PATTERN s60(8,{12,13});
  DEG_SUB_PATTERN s61(-1,{14,15});
    
		      

  DEG_PATTERNS dps = {
    {s00, s01, s02},
    {s10},
    {s20},
    {s30},
    {s40, s41},
    {s50,s51},
    {s60,s61}    
  };

  TEXT t = {4,4,2,1,2,3,2,4,5,1,2,3,4,5};


  EXPECT_EQ (alphabet, dpds.alphabet);
  for (int i=0; i < p.size(); ++i) {
    for (int j=0; j<p[i].size(); ++j) {
      EXPECT_EQ (p[i][j], dpds.patterns[i][j]);
    }
  } 

  for (int i=0; i < ds.size(); ++i) {
    for (int j=0; j<ds[i].size(); ++j) {
      EXPECT_EQ (ds[i][j], dpds.deg_symbols[i][j]);
    }
  } 

  for (int i=0; i < dps.size(); ++i) {
    DEG_PATTERN dp = dps[i];
    for (int j=0; j<dp.size(); ++j) {
      assert (dp[j].first == dpds.deg_patterns[i][j].first);
      for (int k=0; k < dp[j].second.size(); ++k) {
	EXPECT_EQ(dp[j].second[k], dpds.deg_patterns[i][j].second[k]);
      }
    }
  } 
}

TEST(parserTest, validDegenerateText) {
  DEG_TEXT_DS dtds;
  Parser::parse_degenerate_text("test_files/sampleTextDeg.txt",dtds);

  ALPHABET alphabet = "acgt";
  // catta(a,g)gagc(t,g)cttta
  TEXT t = {1,0,3,3,0,4,2,0,2,1,5,1,3,3,3,0};

  DEG_SYMBOLS ds = {
    {1,0,1,0},
    {0,0,1,1}    
  };

  EXPECT_EQ (alphabet, dtds.alphabet);
  for (int i=0; i < t.size(); ++i) {
    EXPECT_EQ (t[i], dtds.substituted[i]);
  } 

  for (int i=0; i < ds.size(); ++i) {
    for (int j=0; j<ds[i].size(); ++j) {
      EXPECT_EQ (ds[i][j], dtds.deg_symbols[i][j]);
    }
  } 

}



