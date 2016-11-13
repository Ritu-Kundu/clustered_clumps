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

/** Defines some helper functions */

#include "../include/utilDefs.hpp"

namespace cluclu{

  static struct option long_options[] =
    {
      { "search",                  required_argument, NULL, 's' },
      { "text-input-file",         required_argument, NULL, 't' },
      { "pattern-input-file",      required_argument, NULL, 'p' },
      { "output-file",             required_argument, NULL, 'o' },
      { "help",                    no_argument,       NULL, 'h' },
      { NULL,                      0,                 NULL, 0   }
    };

  /** Decode the input flags
   */
  ReturnStatus decodeFlags(int argc, char* argv [], struct InputFlags& flags){
    int args = 0;
    int opt;
    std::string alph;
    /* initialisation */
    while ((opt = getopt_long(argc, argv, "s:t:p:o:h", long_options, nullptr)) != - 1) {
      switch (opt) {
      case 's':
	alph = std::string(optarg);
	if (alph == "A") {
	  flags.search_type = SearchType::A;
	}
	else if (alph == "B") {
	  flags.search_type = SearchType::B;
	}
	else if (alph == "C") {
	  flags.search_type = SearchType::C;
	}

	else {
	  std::cerr << "Invalid command: wrong search type: " << std::endl;  
	  return (ReturnStatus::ERR_ARGS);

	}
	args ++;
	break;	

      case 't':
	flags.text_input_filename =  std::string(optarg);
	args ++;
	break;
	
      case 'p':
	flags.pattern_input_filename =  std::string(optarg);
	args ++;
	break;

      case 'o':	
	flags.output_filename = std::string(optarg);
	args ++;
	break;
	

      case 'h':
	return (ReturnStatus::HELP);
      }
    }
    if ( args < 4 ){
      std::cerr << "Invalid command: Too few arguments: " << std::endl;  
      return (ReturnStatus::ERR_ARGS);
    }
    else{
      return (ReturnStatus::SUCCESS);
    }
  }

  /* 
   * Usage of the tool 
   */
  void usage(void) {
    std:: cout << " Usage: cluclu <options>\n";
    std::cout << " Standard (Mandatory):\n";
    std::cout <<  "  -s, --search \t <str> \t \t `A' for Problem-1 (Solid Text and Degenerate Patterns) or `B' for Problem-2 (Degenerate Text and Solid Patterns) or `c' for Problem-3 (Degenerate Text and Degenerate Patterns). \n" ;
    std::cout <<  "  -t, --text-input-file \t <str> \t \t Input file  name for text (Elastic-degenerate string).\n";
    std::cout <<  "  -p, --pattern-input-file \t <str> \t \t Input file  name for (degenerate) patterns.\n";
    std::cout <<  "  -o, --output-file \t <str> \t \t Output filename.\n";
  }


  double gettime(void) {
    struct timeval ttime;
    gettimeofday( &ttime , 0 );
    return ttime.tv_sec + ttime.tv_usec * 0.000001;
  }


} // end namespace
