#ifndef COMMANDS_H
#define COMMANDS_H
#include <string>
#include <iosfwd>
#include <map>
#include <vector>
#include <set>

namespace kolosovskaya
{
  using enW = std::string;
  using ruW = std::set< std::string >;
  using Dict = std::map< enW, ruW >;
  using Dicts = std::map< std::string, Dict >;
  void doPrint(const Dicts&, std::istream&, std::ostream&);
  void doComplement(Dicts&, std::istream&, std::ostream&);
  void doUnion(Dicts&, std::istream&, std::ostream&);
  void doLoad(const Dicts&, std::istream&, std::ostream&);
  void doViewDicts(const Dicts&, std::istream&, std::ostream&);
  void doTranslate(Dicts& allDicts, std::istream& in, std::ostream& out);
  void doAdd(Dicts& allDicts, std::istream& in, std::ostream& out);
}

#endif
