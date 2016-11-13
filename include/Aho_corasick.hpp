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

/** Defines the class Subgraph.
 * It represents a subgraph (which inherits from the class Graph), creates an underlying DAG, and also assists in filtering out 'unreal' superbubbles.
 */

#ifndef AHOCORASICK_HPP
#define AHOCORASICK_HPP

#include <cassert>
#include <queue>
#include <bitset>

#include "globalDefs.hpp"


namespace cluclu{

  class Aho_corasick{
    // State id is index(zero-based) in the vector of states
    using STATE_ID = INT;
    using OUTPUT = std::vector<bool>;
    using STATE = struct State {
      std::vector<STATE_ID> transition;
      STATE_ID failure;
      OUTPUT output;
    };
    using QUEUE = std::queue<STATE_ID>;

  
  public:
    Aho_corasick(const PATTERNS& patterns, const INT alphabet_size);
    OCCURRENCES search(const TEXT &t);
    //////////////////////// private ////////////////////////
  private:
    const INT _cAlphabet_size;
    const INT _cNum_patterns;
    const INT _cSentinel = -1;
    const PATTERNS& _patterns;

    std::vector<STATE> _states;

    void build_trie();
    void build_failure();
    STATE_ID find_target(STATE_ID sid, INT c);
  };

  
} // end namespace
#endif
