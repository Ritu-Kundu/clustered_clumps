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

/** Implements class Elastic_string
 */
#include "../include/Aho_corasick.hpp"

namespace cluclu {

  Aho_corasick::Aho_corasick(const PATTERNS& patterns, const INT alphabet_size)
    : _patterns(patterns)
    , _cNum_patterns(patterns.size())
    , _cAlphabet_size(alphabet_size) {
    build_trie();
    build_failure();
  }

  OCCURRENCES Aho_corasick::search(const TEXT &t) {
    OCCURRENCES occs (t.size());
    Aho_corasick::STATE_ID current = 0; // start from the initial state
    for (INT i=0; i<t.size(); ++i) {
      current = find_target(current, t[i]);
      occs[i] = _states[current].output;
    }
    return occs;
  }



  //////////////////////// private ////////////////////////
  // Assumes alphabet is zero-based integers upto given maximum size
  void Aho_corasick::build_trie() {
    // Add initial state
    Aho_corasick::STATE initial {std::vector<STATE_ID>(_cAlphabet_size, _cSentinel)
	, _cSentinel
	, Aho_corasick::OUTPUT(_cNum_patterns, false)};
    _states.push_back(std::move(initial));

    // Build the trie
    for (INT i= 0; i < _patterns.size(); ++i) {
      const PATTERN& p = _patterns[i];
      Aho_corasick::STATE_ID current_state = 0;
      for (auto c: p) {
	assert (c < _cAlphabet_size);
	if (_states[current_state].transition[c] == _cSentinel) {
	  // Allocate new state
	  _states[current_state].transition[c] = _states.size();
	  Aho_corasick::STATE temp {std::vector<STATE_ID>(_cAlphabet_size, _cSentinel)
	      , _cSentinel
	      , Aho_corasick::OUTPUT(_cNum_patterns, false)};
	  _states.push_back(std::move(temp));
	}
	current_state = _states[current_state].transition[c];
      } // pattern ends
      _states[current_state].output[i] = true;
    } // all patterns end

    // Initial State(0) should have an outgoing edge for all characters.
    for (INT c =0; c < _cAlphabet_size ; ++c) {
      if (_states[0].transition[c] == _cSentinel) {
	// Allocate new state
	_states[0].transition[c] = 0;
      }
    }    
  }

  void Aho_corasick::build_failure() {
    QUEUE q;
    // All nodes of depth 1 have failure pointing to initial (0)
    for (INT c= 0; c < _cAlphabet_size ; ++c) {
      if (_states[0].transition[c] != 0) { // found successor
	Aho_corasick::STATE_ID tsid = _states[0].transition[c]; // target state
	_states[tsid].failure = 0;
	q.push(tsid);
      }
    }

    while (!q.empty()) {
      Aho_corasick::STATE_ID sid = q.front();
      q.pop();
      for (INT c= 0; c < _cAlphabet_size ; ++c) {
	if (_states[sid].transition[c] != _cSentinel) { // found successor
	  Aho_corasick::STATE_ID tsid = _states[sid].transition[c]; // target state
	  Aho_corasick::STATE_ID fsid = _states[sid].failure; // failure state
	  while (_states[fsid].transition[c] == _cSentinel) {
	    fsid = _states[fsid].failure;
	  }
	  Aho_corasick::STATE_ID tsfid = _states[fsid].transition[c]; // failure fro target
	  _states[tsid].failure = tsfid;
	  q.push(tsid);
	  // Merge output
	  for (INT i=0; i < _cNum_patterns; ++i) {
	    if (_states[tsfid].output[i]) {
	      _states[tsid].output[i] = true;
	    }
	  }
	  
	  
	} // failure set for the successor
      } // successor for each char checked
    } // queue ended
  }

  Aho_corasick::STATE_ID Aho_corasick::find_target(Aho_corasick::STATE_ID sid, INT c) {
    assert (c < _cAlphabet_size);
    Aho_corasick::STATE_ID temp = sid;
    while (_states[temp].transition[c] == _cSentinel) {
      temp = _states[temp].failure;
    }
    return (_states[temp].transition[c]);
  }
  


}// end namespace
