#include <limits.h>
#include <vector>
#include <string>
#include <iterator>
#include "globalDefs.hpp"
#include "Aho_corasick.hpp"
#include "gtest/gtest.h"

// TODO: Add test invoving empty seed and/or string in a symbol
using namespace cluclu;
TEST(ahoCorasickTest, simple) {
  cluclu::PATTERNS p = {
    {1,2,3},
    {2,3},
    {4,5} 
  };

  TEXT t = {4,4,2,1,2,3,2,4,5,1,2,3,4,5};

  OCCURRENCES answer = {
    {false, false, false},
    {false, false, false},
    {false, false, false},
    {false, false, false},
    {false, false, false},
    {true, true, false},
    {false, false, false},
    {false, false, false},
    {false, false, true},
    {false, false, false},
    {false, false, false},
    {true, true, false},
    {false, false, false},
    {false, false, true}
  };
  Aho_corasick A(p, 6);
  OCCURRENCES result =  A.search(t);

  for (int i=0; i < t.size(); ++i) {
    for (int j=0; j<p.size(); ++j) {
      EXPECT_EQ(answer[i][j], result[i][j]);
    }
   } 
}


