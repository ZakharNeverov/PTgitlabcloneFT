#ifndef READING_HPP
#define READING_HPP

#include <map>
#include <string>

namespace bazzhin
{
  using dictionary = std::map< std::string, int >;

  dictionary readDictionary(std::istream&);
  void addWordAtDictionary(std::string&, dictionary&);
}

#endif
