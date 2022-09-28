#ifndef COMMANDS_HPP_FT
#define COMMANDS_HPP_FT

#include <iosfwd>
#include "english_dicts.hpp"

namespace dobronravov
{
  void doUnion(std::string, dicts_t&);
  void doIntersect(std::string, dicts_t&);
  void doComplement(std::string, dicts_t&);
  void doLoad(std::string, dicts_t&);
  void doTranslate(std::string, dicts_t&, std::ostream& out);
  void doRemove(std::string, dicts_t&);
  void doCreateDict(std::string, dicts_t&);
  void doPrint(std::string, dicts_t&, std::ostream&);
}

#endif
