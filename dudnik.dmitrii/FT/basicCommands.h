#ifndef BASIC_COMMANDS_H
#define BASIC_COMMANDS_H

#include <map>
#include <iosfwd>
#include <list>
#include <string>
namespace dudnik
{
  using Dict = std::map< int, std::string >;
  using AllDicts = std::map< std::string, Dict >;
  std::ostream& basicPrint(std::ostream&, const std::string&, const Dict&);
  Dict basicComplement(const Dict&, const Dict&);
  Dict basicIntersect(const Dict&, const Dict&);
  Dict basicUnite(const Dict&, const Dict&);
}

#endif
