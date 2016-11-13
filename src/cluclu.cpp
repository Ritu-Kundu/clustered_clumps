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

/** Module containing main() method.
 */

#include <fstream>
#include <cstdlib>

#include "../include/globalDefs.hpp"
#include "../include/Aho_corasick.hpp"
#include "../include/Parser.hpp"
#include "../include/Search.hpp"
#include "../include/utilDefs.hpp"
#include <sdsl/suffix_arrays.hpp>
/** Module containing main() method.
 */

using namespace cluclu;

int main(int argc, char **argv){
  /* Decode arguments */
  struct InputFlags flags;
  if(decodeFlags (argc, argv, flags ) != ReturnStatus::SUCCESS){
    usage();
    return 1;
  }
  /* Parse the input */
  INT text_size=0;
  INT pattern_size=0;
  INT num_patterns=0;
  INT d=0;
  INT alphabet_size = 0;

  ReturnStatus rs;
  // Parse Patterns
  DEG_PATTERNS_DS dpds;
  Parser::parse_degenerate_patterns(flags.pattern_input_filename, dpds);
  if (rs != ReturnStatus::SUCCESS) {
    return static_cast<int>(rs);
  }
  num_patterns = dpds.deg_patterns.size();
  d = dpds.deg_symbols.size();
  pattern_size = d;
  alphabet_size = dpds.alphabet.size();
    for (auto&& p: dpds.patterns) {
    pattern_size += p.size();
  }

  // Parse Text
  TEXT t;
  DEG_TEXT_DS dtds;  

  switch (flags.search_type) {
  case SearchType::A:
    rs =  Parser::parse_simple_text(flags.text_input_filename, dpds.alphabet, t);
    text_size = t.size();
    break;
  case SearchType::B:
  case SearchType::C:
    rs =  Parser::parse_degenerate_text(flags.text_input_filename, dtds);
    text_size = dtds.substituted.size();
    d += dtds.deg_symbols.size(); 
    break;   
  }  
  if (rs != ReturnStatus::SUCCESS) {
    return static_cast<int>(rs);
  };
  

  /* Find the clustered-clumps */
  CLUSTERED_CLUMPS result;
  double start = gettime();
  switch (flags.search_type) {
  case SearchType::A:
    result = Search::find_TypeA(dpds, t);
    break;
  case SearchType::B:
    result = Search::find_TypeB(dpds.patterns, dtds);
    break;
  case SearchType::C:
    assert(dpds.alphabet == dtds.alphabet);
    result = Search::find_TypeC(dpds, dtds);
    break;   
  }  
  double end = gettime();

  /* Report */
  std::string filename = flags.output_filename;
  std::ofstream outfile(filename);
  std::cout << "Writing result in file: " << filename << std::endl;
  if(!outfile.is_open()){
    std::cerr << "Cannot open  file \n";
    return static_cast<int>(ReturnStatus::ERR_FILE_OPEN);
  }
  outfile << "Size of the alphabet: " << alphabet_size << "\n";
  outfile << "Number of patterns: " << num_patterns << "\n";
  outfile << "Total size of the patterns: " << pattern_size << "\n";
  outfile << "Size of the text: " << text_size << "\n";
  outfile << "d (Total number of degenerate/non-solid positions): " << d << "\n";
  outfile << "Number of clustred-clumps: " <<result.size() << "\n";
  outfile << "Time Elapsed for processing: " << (end - start)<< " secs.\n";

  for (auto&& c:result) {
    outfile << "(" << c.first << "," << c.second << ") ; ";
  }
  outfile << "\n\n";
  
}
