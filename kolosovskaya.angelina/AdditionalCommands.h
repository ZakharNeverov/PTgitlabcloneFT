#ifndef ADDITIONAL_COMMANDS_H
#define ADDITIONAL_COMMANDS_H
#include "Commands.h"
#include <iosfwd>
#include <vector>
#include <string>

namespace kolosovskaya
{
  Dict makeDict(std::istream&, std::ostream& out);
  Dicts getDicts(std::string&, std::ostream& out);
  ruW getRuWords(std::string&);
  std::ostream& printDict(std::ostream&, const std::string&, const Dicts&);
  void printRuWords(std::ostream&, const kolosovskaya::ruW&);
  std::vector< std::string > getNamesFromString(std::string, char);
  ruW complementRuWords(const ruW&, const ruW&);
  Dict complementDicts(const Dict&, const Dict&);
  ruW makeUnionRuWords(const ruW&, const ruW&);
  Dict makeUnion(const Dict&, const Dict&);
  bool doesExist(const std::string&, const Dicts&);
}

#endif

