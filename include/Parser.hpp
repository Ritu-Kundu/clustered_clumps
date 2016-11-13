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

/** @file Parser.hpp 
 * @brief Defines the class Parser.
 * It reads the file into the data-structures required for Search to work.
 */
#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <cctype>

#include "globalDefs.hpp"

namespace cluclu{
  /** Class Parser
   * A Parser contains static methods for parsing elastic-degenerate and (standard) degenerate string.
   */
  class Parser{
    enum class CollectionMode {
      SOLID,
	DEG
    };

  public:
   static ReturnStatus parse_degenerate_patterns(const std::string filename, DEG_PATTERNS_DS& dpds);
   static ReturnStatus parse_simple_text(const std::string filename, const ALPHABET& alphabet, TEXT& text);
    static ReturnStatus parse_degenerate_text(const std::string filename, DEG_TEXT_DS& dtds);
  private:
 
  };

} // end namespace
#endif
