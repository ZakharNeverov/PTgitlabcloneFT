#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <iosfwd>
#include <list>
#include "basicCommands.h"
namespace dudnik
{
  using AllDicts = std::map< std::string, Dict >;
  using Args = std::list< std::string >;
  using Keys = std::list< int >;
  std::ostream& print(std::ostream&, const Args&, const AllDicts&);
  std::ostream& printAll(std::ostream&, const AllDicts&);
  Dict complement(const Args&, const AllDicts&);
  Dict intersect(const Args&, const AllDicts&);
  Dict unite(const Args&, const AllDicts&);
  Dict uniteAll(const AllDicts&);
  void replace(const std::string&, int, const std::string&, AllDicts&);
  void remove(const std::string&, const Keys&, AllDicts&);
  void remove(const Args&, AllDicts&);
  bool equal(const Args&, const AllDicts&);
  void insert(const std::string&, int, const std::string&, AllDicts&);
  void clear(const Args&, AllDicts&);
  void clearAll(AllDicts&);
}

#endif
