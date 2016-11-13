/**
    ClusteredClumps: Finding Clustered-Clumps in Degenerate Strings
    Copyright (C) 2016 Ritu Kundu, Manal Mohamed
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

/** Defines the class Search.
 * It provides methods to perform three types of search:
 */

#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <cassert>
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/lcp.hpp>
#include <sdsl/rmq_support.hpp>
#include <iostream>

#include "globalDefs.hpp"
#include "Aho_corasick.hpp"

namespace cluclu{

  class Search{
    struct SearchB_DS {
      sdsl::csa_bitcompressed<> csa;
      sdsl::lcp_bitcompressed<> lcp;
      sdsl::rmq_succinct_sct<> rmq;    
    };
    struct SearchIndex {
      const PATTERN&  pattern; // reference to string 
      const INT base_index; // starting-index in the generalised string
      const INT offset; // index within the string (zero-based)
    };

    using START_INDICES = std::vector<INT>;

  public:
    static CLUSTERED_CLUMPS find_TypeA(const DEG_PATTERNS_DS& dpds, const TEXT& t);
    static CLUSTERED_CLUMPS find_TypeB(const PATTERNS& p, const DEG_TEXT_DS& dtds);
    static CLUSTERED_CLUMPS find_TypeC(const DEG_PATTERNS_DS& dpds, const DEG_TEXT_DS& dtds);

    //////////////////////// private ////////////////////////
  private:
    static LONGEST_PATTERN_OCC compute_longest_Occ(const DEG_PATTERNS_DS& dpds, const DEG_TEXT_DS& dtds, const OCCURRENCES& occs);
    
    static bool deg_region_match(const DEG_SYMBOLS& deg_symbols, const DEG_REGION& dr, const DEG_TEXT_DS& dtds, INT pos);
    
    static void preprocess_degenerate_text(const DEG_TEXT_DS& dtds, const PATTERNS& p, START_INDICES& start_indices, SearchB_DS& searchds);
    static void prepare_for_lcp(const PATTERN& generalised, SearchB_DS& searchds);
    
    static INT get_longest_match(const DEG_TEXT_DS& dtds, const SearchB_DS& searchDS, const SearchIndex& si1, const SearchIndex& si2);
    static bool is_degenerateT_solidP_match(const DEG_TEXT_DS& dtds, INT tc, INT pc);
    static INT getLCP(const INT suff1, const INT suff2, const SearchB_DS& searchds);
    
    static CLUSTERED_CLUMPS compute_clustered_clumps(const LONGEST_PATTERN_OCC& longest);
  };

  
} // end namespace
#endif
