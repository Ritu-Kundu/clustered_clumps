/**
   ElDeS: Pattern Matching in Elastic-Degenerate strings
   Copyright (C) 2016 Ritu Kundu, Solon P. Pissis 
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

/** Implements class Parser
 */
#include "../include/Parser.hpp"

namespace cluclu {
  ReturnStatus Parser::parse_degenerate_patterns(const std::string filename, DEG_PATTERNS_DS& dpds) {
    std::ifstream infile(filename);
    std::cout << "Reading file: " << filename << std::endl;
    if(!infile.is_open()){
      std::cerr << "Cannot open input file \n";
      return ReturnStatus::ERR_FILE_OPEN;
    }
    std::string line;
    // Get the first line: representing alphabet;
    getline(infile, line);
    if (line.empty()) {
      std::cerr << "No Input: Empty File: " << std::endl;
      return ReturnStatus::ERR_INVALID_INPUT;
    }
    // Assign alphabet
    dpds.alphabet = line;
    const INT cAlphabet_size = line.size();
    
    int lineNum = 1;
    while (getline(infile, line)) {
      ++ lineNum;
      if (!line.empty()) {
	if (line[0] != '>') {
	  std::cerr << "Invalid Input: Expected '>' at line number" << lineNum << std::endl;
	  return ReturnStatus::ERR_INVALID_INPUT;
	}
	// Pattern starts
	DEG_PATTERN deg_pattern;
	PATTERN solid_subpattern;
	DEG_REGION dgr;
	int row=-1;
	Parser::CollectionMode mode = Parser::CollectionMode::DEG;
	while (getline(infile, line)) {
	  ++ lineNum;
	  if (line.empty()) {
	    break; // empty line => sequence ends
	  }
	  int column = -1;
	  int num_letters=0;
	  int letter=-1;
	  ++row;
	  DEG_SYMBOL deg_symb(cAlphabet_size, false);
	  
	  for (char c: line) {
	    if (isspace(c)) {
	      // Ignore
	    }
	    else if (c == '1' || c== '0') { // valid characters
	      ++column;
	      if (column >= cAlphabet_size) { // error
		std::cerr << "Invalid Input: More characters than in alphabet at line number" << lineNum << std::endl;
		return ReturnStatus::ERR_INVALID_INPUT;
	      }
	      if (c == '1') {
		  ++num_letters;
		  letter = column;
		  deg_symb[column] = true;
	      }
	    }
	    else {
	      std::cerr << "Invalid Input: Invalid character: " << c <<  "at line number" << lineNum << std::endl;
	      return ReturnStatus::ERR_INVALID_INPUT;
	    }
	  } // one row (position in the pattern) ends
	  
	  if  (column < cAlphabet_size-1) { // less indicators at this position
	    std::cerr << "Invalid Input: Less number of letters at line number" << lineNum << std::endl;
	    return ReturnStatus::ERR_INVALID_INPUT;
	  }
	  else if  (num_letters == 0) { // no letter at this position
	    std::cerr << "Invalid Input: No letter at line number" << lineNum << std::endl;
	    return ReturnStatus::ERR_INVALID_INPUT;
	  }
	  else if (num_letters == 1) { // solid position
	    if (mode == Parser::CollectionMode::DEG) { // a solid subpattern starts, change mode
	      mode  = Parser::CollectionMode::SOLID;
	    }
	    solid_subpattern.push_back(letter);
	  }
	  else { // non-solid position
	    if (mode == Parser::CollectionMode::SOLID) {// was collecting solid string	     
	      // change mode
	      mode  = Parser::CollectionMode::DEG;

	      // solid subpattern finished, add to pattern, add its index and dgr into deg-subpattern
	      INDEX ind;
	      if (!solid_subpattern.empty()) {
		dpds.patterns.push_back(std::move(solid_subpattern));
		ind = dpds.patterns.size()-1;
	      }	     
	      else {
		ind = cInvalid_index;
	      }
	      DEG_SUB_PATTERN dsp { ind, std::move(dgr)};
	      // deg-subpattern completed, add to deg-pattern sequence
	      deg_pattern.push_back(std::move(dsp));
	    }
	    // add deg-symbol in the vector
	    dpds.deg_symbols.push_back(std::move(deg_symb));
	    // add its index into deg-region
	    dgr.push_back(dpds.deg_symbols.size()-1);
	  }
	} // pattern ends
	// Add last subpattern
	// solid subpattern finished, add to pattern, add its index and dgr into deg-subpattern
	INDEX ind;
	if (!solid_subpattern.empty()) {
	  dpds.patterns.push_back(std::move(solid_subpattern));
	  ind = dpds.patterns.size()-1;
	}	     
	else {
	  ind = cInvalid_index;
	}
	DEG_SUB_PATTERN dsp { ind, std::move(dgr)};
	// deg-subpattern completed, add to deg-pattern sequence
	deg_pattern.push_back(std::move(dsp));

	// Add pattern
	dpds.deg_patterns.push_back(std::move(deg_pattern));
      }
    } // sequence ends
    
    std::cout << "File read successfully: " << filename << std::endl;
    return ReturnStatus::SUCCESS;

  }
  
  ReturnStatus Parser::parse_simple_text(const std::string filename, const ALPHABET& alphabet, TEXT& text){
    std::ifstream infile(filename);
    std::cout << "Reading file: " << filename << std::endl;
    if(!infile.is_open()){
      std::cerr << "Cannot open input file \n";
      return ReturnStatus::ERR_FILE_OPEN;
    }
    std::string line;
    // Get the first line: representing alphabet;
    getline(infile, line);
    if (line.empty()) {
      std::cerr << "No Input: Empty File: " << std::endl;
      return ReturnStatus::ERR_INVALID_INPUT;
    }
    
    int lineNum = 0;
    do {
      ++ lineNum;
      if (line.empty()) {
	break; // empty line => sequence ends
      }
      INT mapped_char;
      for (char c: line) {
	if (isspace(c)) {
	  // Ignore
	}
	else if ((mapped_char = alphabet.find(c)) != std::string::npos) { // valid characters
	  text.push_back(mapped_char);
	}
	else {
	  std::cerr << "Invalid Input: Invalid character: " << c <<  "at line number" << lineNum << std::endl;
	  return ReturnStatus::ERR_INVALID_INPUT;
	}
      } // one row ends
	  
    } while (getline(infile, line)); // sequence ends
    
    std::cout << "File read successfully: " << filename << std::endl;
    text.shrink_to_fit();
    return ReturnStatus::SUCCESS;

  }

  ReturnStatus Parser::parse_degenerate_text(const std::string filename, DEG_TEXT_DS& dtds) {
    std::ifstream infile(filename);
    std::cout << "Reading file: " << filename << std::endl;
    if(!infile.is_open()){
      std::cerr << "Cannot open input file \n";
      return ReturnStatus::ERR_FILE_OPEN;
    }
    std::string line;
    // Get the first line: representing alphabet;
    getline(infile, line);
    if (line.empty()) {
      std::cerr << "No Input: Empty File: " << std::endl;
      return ReturnStatus::ERR_INVALID_INPUT;
    }
    // Assign alphabet
    dtds.alphabet = line;
    const INT cAlphabet_size = line.size();
    
    // Get identifier: currently ignored
    getline(infile, line);
    int lineNum = 2;
    if (!line.empty()) {
      if (line[0] != '>') {
	std::cerr << "Invalid Input: Expected '>' at line number " << lineNum << std::endl;
	return ReturnStatus::ERR_INVALID_INPUT;
      }

      // Pattern starts
      int row=-1;
      INT delim = cAlphabet_size;
      while (getline(infile, line)) {
	++ lineNum;
	if (line.empty()) {
	  break; // empty line => sequence ends
	}
	int column = -1;
	int num_letters=0;
	int letter=-1;
	++row;
	DEG_SYMBOL deg_symb(cAlphabet_size, false);
	  
	for (char c: line) {
	  if (isspace(c)) {
	    // Ignore
	  }
	  else if (c == '1' || c== '0') { // valid characters
	    ++column;
	    if (column >= cAlphabet_size) { // error
	      std::cerr << "Invalid Input: More characters than in alphabet at line number" << lineNum << std::endl;
	      return ReturnStatus::ERR_INVALID_INPUT;
	    }
	    if (c == '1') {
	      ++num_letters;
	      letter = column;
	      deg_symb[column] = true;
	    }
	  }
	  else {
	    std::cerr << "Invalid Input: Invalid character: " << c <<  " at line number" << lineNum << std::endl;
	    return ReturnStatus::ERR_INVALID_INPUT;
	  }
	} // one row (position in the pattern) ends
	  
	if  (column < cAlphabet_size-1) { // less indicators at this position
	  std::cerr << "Invalid Input: Less number of letters at line number" << lineNum << std::endl;
	  return ReturnStatus::ERR_INVALID_INPUT;
	}
	else if  (num_letters == 0) { // no letter at this position
	  std::cerr << "Invalid Input: No letter at line number" << lineNum << std::endl;
	  return ReturnStatus::ERR_INVALID_INPUT;
	}
	else if (num_letters == 1) { // solid position
	  dtds.substituted.push_back(letter);
	}
	else { // non-solid position
	  // add deg-symbol in the vector
	  dtds.deg_symbols.push_back(std::move(deg_symb));
	  // add unique identifier to the pattern
	  if  (delim > 254) { // no letter at this position
	    std::cerr << "Invalid Input: More number of degenerate symbols than () at line number () : " << 255-cAlphabet_size << " : " << lineNum << std::endl;
	    return ReturnStatus::ERR_INVALID_INPUT;
	  }
	  dtds.substituted.push_back(delim++);
	}
      } // pattern ends
      std::cout <<std::endl;
    }
    
    std::cout << "File read successfully: " << filename << std::endl;
    return ReturnStatus::SUCCESS;

  }
  //////////////////////// private ////////////////////////



} // end namespace
