#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "io_dicts.hpp"

namespace kurzov
{
  using istream_t = std::istream;
  bool doPrint(istream_t&, dicts_t&, std::ostream&);
  bool doUnion(istream_t&, dicts_t&);
  bool doComplement(istream_t&, dicts_t&);
  bool doIntersect(istream_t&, dicts_t&);
  bool doLoad(istream_t&, dicts_t&);
  bool doTranslate(istream_t&, dicts_t&, std::ostream&);
}

#endif
