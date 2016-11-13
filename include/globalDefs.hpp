/**
   ClusteredClumps: Finding Clustered-Clumps in Degenerate Strings
   Copyright (C) 2016 Ritu Kundu,Manal Mohamed
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

/** Declaration used by each of the other modules
 */

#ifndef GLOBAL_DEFS
#define GLOBAL_DEFS

#include <cstdint>
#include <stdio.h>
#include <iterator>
#include <tuple>
#include <string>
#include <vector>
#include <list>
#include <iostream>


namespace cluclu{
#define DEBUG

  //using UINT = int16_t;
  using INT = int64_t;
  
 
  enum class ReturnStatus{
    SUCCESS,
      ERR_ARGS,
      ERR_FILE_OPEN,
      ERR_INVALID_INPUT,
      ERR_INVALID_INDEX,
      ERR_LIMIT_EXCEEDS,
      HELP
      };
  enum class SearchType{
    A,
      B,
      C
      };
  
  using ALPHABET = std::string;
  
  using PATTERN = std::vector<INT>;
  using PATTERNS = std::vector<PATTERN>;
  using TEXT = PATTERN;

  using OCCURRENCE = std::vector<bool>;
  using OCCURRENCES = std::vector<OCCURRENCE>;

  using INDEX = INT; // Index in corresponding vector: Solid subpatterns or Deg symbols
  const INT cInvalid_index = -1;
  using DEG_REGION = std::vector<INDEX>; // Indices of symbols in the degenerate region
  using DEG_SUB_PATTERN = std::pair<INDEX, DEG_REGION>; // Index of the solid sub pattern (-1 if empty) and the preceding degenerate region (may be empty).
  using DEG_PATTERN = std::vector<DEG_SUB_PATTERN>;
  using DEG_PATTERNS = std::vector<DEG_PATTERN>;

  using DEG_SYMBOL = std::vector<bool>;
  using DEG_SYMBOLS = std::vector<DEG_SYMBOL>; // bool matrix (D * alphabet_size)

  using DEG_PATTERNS_DS = struct Deg_Patterns_DS {
    ALPHABET alphabet;
    PATTERNS patterns;
    DEG_SYMBOLS deg_symbols;
    DEG_PATTERNS deg_patterns;
  };

  using DEG_TEXT_DS = struct Deg_Text_DS {
    ALPHABET alphabet;
    TEXT substituted;
    DEG_SYMBOLS deg_symbols;
  };
  using LONGEST_PATTERN_OCC = std::vector<INT>; 
    
  using CLUSTERED_CLUMP = std::pair<INT, INT>; // (start-index, length)
  using CLUSTERED_CLUMPS = std::vector<CLUSTERED_CLUMP>;

} // end namespace

#endif
