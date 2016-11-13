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

/** Implements class Search
 */
#include "../include/Search.hpp"

namespace cluclu {
  CLUSTERED_CLUMPS Search::find_TypeA(const DEG_PATTERNS_DS& dpds, const TEXT& t) {
    const PATTERNS& p = dpds.patterns;
    const INT alphabet_size = dpds.alphabet.size();

    // find occurrences of solid subpatterns
    Aho_corasick A(p, alphabet_size);
    OCCURRENCES occs =  A.search(t);

    // find occurrence of the longest pattern at each position
    DEG_TEXT_DS dtds{dpds.alphabet, t, DEG_SYMBOLS{}};
    LONGEST_PATTERN_OCC longest = compute_longest_Occ(dpds, dtds, occs);
    
    // compute clustered clumps
    return compute_clustered_clumps(longest);    
  }

  CLUSTERED_CLUMPS Search::find_TypeB(const PATTERNS& p, const DEG_TEXT_DS& dtds) {
    const INT alphabet_size = dtds.alphabet.size();
    const TEXT& t = dtds.substituted;
    // preprocess the degenerate text
    Search::START_INDICES start_indices(p.size()); // text followed by each of the patterns; text starts at 0
    Search::SearchB_DS sds{};
    preprocess_degenerate_text(dtds, p, start_indices, sds);
      
    // search patterns
    LONGEST_PATTERN_OCC longest(t.size(), 0);
    const INT cStartDelimiter = dtds.alphabet.size();
    for (INT i=0; i < t.size(); ++i) {
      for (INT j=0; j < p.size(); ++j) {
	SearchIndex si1{t, 0, i};
	SearchIndex si2{p[j], start_indices[j], 0};
	INT match = get_longest_match(dtds, sds, si1, si2);
	if (match == p[j].size()) { // match found
	  if (match > longest[i]) {
	    longest[i] = match;
	  }
	}
      } // each pattern checked      
    } // each text-position checked

    // compute clustered clumps    
    return compute_clustered_clumps(longest);    
  }

  // Assumes alphabets of noth are the same
  CLUSTERED_CLUMPS Search::find_TypeC(const DEG_PATTERNS_DS& dpds, const DEG_TEXT_DS& dtds) {
    assert(dpds.alphabet == dtds.alphabet);
    const PATTERNS& p = dpds.patterns;
    const INT alphabet_size = dtds.alphabet.size();
    const TEXT& t = dtds.substituted;
    // preprocess the degenerate text
    Search::START_INDICES start_indices(p.size()); // text followed by each of the patterns; text starts at 0
    Search::SearchB_DS sds{};
    preprocess_degenerate_text(dtds, p, start_indices, sds);
      
    // find occurrences of solid subpatterns
    OCCURRENCE occ(p.size(), false);
    OCCURRENCES occs(t.size(), occ);
    const INT cStartDelimiter = dtds.alphabet.size();
    for (INT i=0; i < t.size(); ++i) {
      for (INT j=0; j < p.size(); ++j) {
	SearchIndex si1{t, 0, i};
	SearchIndex si2{p[j], start_indices[j], 0};
	INT match = get_longest_match(dtds, sds, si1, si2);
	if (match == p[j].size()) { // match found
	  occs[i+match-1][j] = true;
	}
      } // each pattern checked      
    } // each text-position checked

    // find occurrence of the longest pattern at each position
    LONGEST_PATTERN_OCC longest = compute_longest_Occ(dpds, dtds, occs);

    // compute clustered clumps
    return compute_clustered_clumps(longest);    
  }





  //////////////////////// private ////////////////////////
  LONGEST_PATTERN_OCC Search::compute_longest_Occ(const DEG_PATTERNS_DS& dpds, const DEG_TEXT_DS& dtds, const OCCURRENCES& occs) {
    const PATTERNS& p = dpds.patterns;
    const TEXT& t = dtds.substituted;
    const DEG_SYMBOLS& deg_symbols = dpds.deg_symbols;
    const DEG_PATTERNS& deg_patterns = dpds.deg_patterns;

    LONGEST_PATTERN_OCC longest(t.size(), 0);
    for (INT i = 0; i < t.size(); ++i) {
      for (auto&& dp: deg_patterns) {
	INT end_pos = i;
	bool flag_fail = false;
	for (auto it = dp.rbegin(); it != dp.rend(); ++it) { // from last subpattern to the first
	  DEG_SUB_PATTERN dsp  = *it;
	  // check both:  condition 1 (solid subpattern exists at this position) and condition 2 (following degenerate region
	  if (dsp.first != cInvalid_index) { // solid subpattern is non-empty
	    if (end_pos >= 0 && occs[end_pos][dsp.first]) { // condition 1 passed
	      end_pos -= (p[dsp.first].size() + dsp.second.size()); // shift to expected end of the previous subpattern
	      if (end_pos < -1 || !deg_region_match(deg_symbols, dsp.second, dtds, end_pos+1)) { //condition 2 fails
		flag_fail = true;
		break;
	      }
	    }
	    else { // condition 1 fails
	      flag_fail = true;
	      break;
	    }
	  }
	  else { // empty solid subpattern
	    end_pos -= dsp.second.size(); // shift to expected end of the previous subpattern
	    if (end_pos < -1 || !deg_region_match(deg_symbols, dsp.second, dtds, end_pos+1)) { //condition 2 fails
		flag_fail = true;
		break;
	      }
	  }	  
	} // each deg-subpattern of the deg-apttern checked or it failed
	if (!flag_fail) { // deg-pattern found
	  INT pattern_len = i - end_pos;
	  if (pattern_len > longest [end_pos+1]) { // longer pattern
	    longest [end_pos+1] = pattern_len;
	  }
	}
      } // each given deg-pattern checked
    } // each text-position checked
    return longest;
    
  }

  // returns true if empty dr
  bool Search::deg_region_match(const DEG_SYMBOLS& deg_symbols, const DEG_REGION& dr, const DEG_TEXT_DS& dtds, INT pos) {
    bool result = true;
    const TEXT& t = dtds.substituted;
    for (auto ind: dr) {
      DEG_SYMBOL pd = deg_symbols[ind];
      bool flag=false;
      for (int i=0; i < dtds.alphabet.size(); ++i) {
	if (pd[i]) {
	  if ( is_degenerateT_solidP_match(dtds, t[pos], i)) { // match
	    flag=true;
	    break;
	  }
	}
      } // all letters of deg p checked
     
      if (!flag) {
	result=false;
	break;
      }
      ++pos;
    }
    return result;
  }

  void Search::preprocess_degenerate_text(const DEG_TEXT_DS& dtds, const PATTERNS& p, Search::START_INDICES& start_indices, Search::SearchB_DS & searchds) {
    const TEXT& t = dtds.substituted;
    // map generalised text to starting indices
    // text followed by each of the patterns; text starts at 0
    start_indices[0] = t.size()+1;
    for (int i=0; i < p.size()-1; ++i) {
      start_indices[i+1] = start_indices[i] + p[i].size() + 1;
    }
    
    // combine text and all the patterns delimited by
    INT total_size = start_indices[start_indices.size()-1]+p[p.size()-1].size()+1;
    TEXT generalised(total_size);
    // NOTE: For SDSL 0 can not be in the string. Therefore, +1 in each
    INT delim = dtds.alphabet.size() + dtds.deg_symbols.size()+1;
    INT ind = 0;
    for(int i=0; i < t.size(); ++i) { // copy text
      generalised[ind++] = t[i]+1;
    }
    generalised[ind++] = delim++;
    for (int i=0; i < p.size(); ++i) {
      for(int j=0; j < p[i].size(); ++j) { // copy text
	assert(p[i][j] < 254);
    	generalised[ind] = p[i][j]+1;
	++ind;
      }
      assert(delim < 254);
      generalised[ind++] =delim++;
    }

    // prepare for constant time LCP
    prepare_for_lcp(generalised, searchds);
  }
  
  void Search::prepare_for_lcp(const PATTERN& generalised, Search::SearchB_DS& searchds) {
    std::ostringstream oss;
    std::move(generalised.begin(), generalised.end(), std::ostream_iterator<int>(oss, " "));
    sdsl::construct_im(searchds.csa, oss.str(), 'd'); // 1 for alphabet type
    //std::cout << " i SA ISA T[SA[i]..SA[i]-1]" << std::endl;
    //sdsl::csXprintf(std::cout, "%2I %2S %3s %:3T", csa);

    sdsl::construct_im(searchds.lcp,  oss.str(), 'd'); // 1 for alphabet type
    //std::cout << lcp << std::endl;
    searchds.rmq = std::move(sdsl::rmq_succinct_sct<> (&(searchds.lcp)));
    // RMQ rmq(&(searchds.lcp));
    // rmq does not need its arg to answer the queries
    //sdsl::util::clear(lcp); // so we can free the space for v
  }
  
  INT Search::get_longest_match(const DEG_TEXT_DS& dtds, const Search::SearchB_DS& searchDS, const Search::SearchIndex& si1, const Search::SearchIndex& si2){
    INT longest_match = 0;
    INT start_index1 = si1. base_index + si1.offset;
    INT start_index2 = si2. base_index + si2.offset;
     
    // Stop just before starting of the next string in the generalised string
    INT stop_index1 = si1. base_index + si1.pattern.size();
    INT stop_index2 = si2. base_index + si2.pattern.size();
    INT lcp = 0;

    while (start_index1 < stop_index1 && start_index2 < stop_index2) {
      lcp = getLCP(start_index1, start_index2, searchDS);
      longest_match += lcp;
      start_index1 += lcp;
      start_index2 += lcp;
      if (start_index1 < stop_index1 && start_index2 < stop_index2) {
	if (is_degenerateT_solidP_match(dtds, si1.pattern[start_index1-si1. base_index], si2.pattern[start_index2-si2. base_index])) { // mismatch only due to degnerate symbol (invalid)
	  ++start_index1;
	  ++start_index2;
	  ++longest_match;
	}
	else { // Valid mismatch
	  break;
	}
      }
    }
    return longest_match;
  }

  bool Search::is_degenerateT_solidP_match(const DEG_TEXT_DS& dtds, INT tc, INT pc) {
    const INT cStartDelimiter = dtds.alphabet.size();
    const DEG_SYMBOLS& cDeg_symbols = dtds.deg_symbols; 
    bool result = false;
    if (tc >= cStartDelimiter) { // non-solid position
      INT ind = tc-cStartDelimiter;
      result = cDeg_symbols[ind][pc];
    }
    else {
      result = (tc == pc);
    }
    return result;
  }
  
  INT Search::getLCP(const INT suff1, const INT suff2, const Search::SearchB_DS& searchds) {
    assert(!searchds.csa.empty());
    assert(!searchds.lcp.empty());
    assert(searchds.rmq.size() > 0);
    INT l_rmq, r_rmq;
    INT r1 = searchds.csa.isa[suff1];
    INT r2 = searchds.csa.isa[suff2];
    if (r1 < r2) {
      l_rmq=r1;
      r_rmq=r2;
    }
    else {
      l_rmq=r2;
      r_rmq=r1;
    }
    INT minIndex = searchds.rmq(l_rmq+1, r_rmq);
    INT lcp = searchds.lcp[minIndex];
    return lcp;
    //std::cout << "LCP: "<< lcp << std::endl;

  }


  CLUSTERED_CLUMPS Search::compute_clustered_clumps(const LONGEST_PATTERN_OCC& longest) {
    CLUSTERED_CLUMPS result;
    INT start=0; // index of beginning of clustered-clump
    INT last=0; // index of end of clustered-clump
    for (INT u=0; u < longest.size(); ++u) {
      if (longest[u] == 0) { // no occurrence
	if (last < u) { // crossed last
	  start = u;
	  last = u;
	}
      }
      else if (u+longest[u] > last) { // longer occurrence found: expanding past last
	last = u+longest[u]-1;
      }
      if (u==last) {
	if (last-start > 0) { // report
	  CLUSTERED_CLUMP c(start, last-start+1);
	  result.push_back(c);
	}
	start = last+1;
	last = last+1;
      }
    }
    return result;
  } 


}// end namespace
