#ifndef STREAM_PROCESSING_H
#define STREAM_PROCESSING_H

#include <string>
#include <map>
#include <list>
#include <iosfwd>

namespace dudnik
{
  using Dict = std::map< int, std::string >;
  using AllDicts = std::map< std::string, Dict >;
  std::list< std::string > getWords(std::istream&, char);
  AllDicts getDictsFromStream(std::istream&);
}

#endif
