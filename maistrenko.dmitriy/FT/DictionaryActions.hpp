#ifndef DICTIONARY_STRUCTURE_HPP
#define DICTIONARY_STRUCTURE_HPP

#include "DictionaryAliases.hpp"

namespace maistrenko
{
  void addWord(Dict&, const std::pair< EngW, RusS >&);
  bool removeWord(Dict&, const EngW&);
  bool removeTranslate(Dict&, const EngW&, const RusW&);

  Dict unionDicts(const Dict&, const Dict&);
  Dict intersectDicts(const Dict&, const Dict&);
  Dict substractDicts(const Dict&, const Dict&);
}

#endif
