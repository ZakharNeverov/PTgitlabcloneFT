#ifndef DICTIONARY_ALIASES_HPP
#define DICTIONARY_ALIASES_HPP

#include <string>
#include <map>
#include <set>

namespace maistrenko
{
  using EngW = std::string;
  using RusW = std::string;
  using RusS = std::set< RusW >;
  using Dict = std::map< EngW, RusS >;
  using Dicts = std::map< std::string, Dict >;
}

#endif
