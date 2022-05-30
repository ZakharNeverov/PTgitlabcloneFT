
#ifndef USERCOMMANDS_HPP
#define USERCOMMANDS_HPP

#include <iosfwd>
#include <string>
#include <vector>
#include "Dictionary.hpp"

namespace shkroba
{
  void makePrint(std::istream& in, std::vector<Dictionary>& dictionaries, std::ostream&);
  void makeSize(std::istream& in, std::vector< Dictionary >& dictionaries, std::ostream& out);
  void makeFindWord(std::istream& in, std::vector< Dictionary >& dictionaries, std::ostream& out);
  void makeOneTranslate(std::istream& in, std::vector< Dictionary >& dictionaries, std::ostream& out);
  void makeUnique(std::istream& in, std::vector< Dictionary >& polygons, std::ostream& out);
  void makeCommonForTwo(std::istream& in, std::vector< Dictionary >& dictionaries, std::ostream& out);
  void makeCommonDictionary(std::istream& in, std::vector< Dictionary >& dictionaries, std::ostream& out);
  void makeHelp(std::istream& in, std::ostream& out);
}

#endif
