#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "io_dicts.hpp"

namespace kurzov
{
  void doPrint(const std::string&, dicts_t&, std::ostream&);
  void doUnion(const std::string&, dicts_t&);
  void doComplement(const std::string&, dicts_t&);
  void doIntersect(const std::string&, dicts_t&);
  void doLoad(const std::string&, dicts_t&);
  void doSave(const std::string&, dicts_t&);
  void doTranslate(const std::string&, dicts_t&, std::ostream&);
}

#endif
