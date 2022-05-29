#ifndef CROSS_REFERENCES_HPP
#define CROSS_REFERENCES_HPP

#include <iostream>
#include <string>
#include <map>
#include <set>

namespace shulgin
{
  using LinesList = std::set< size_t >;
  using TextLinesListDict = std::map< std::string, LinesList >;
  using CrossReferencesDict = std::map< std::string, TextLinesListDict >;
  using WordAndTexts = CrossReferencesDict::value_type;
  using TextAndLinesList = TextLinesListDict::value_type;
  using Texts = std::set< std::string >;

  struct CrossReferences
  {
    CrossReferencesDict dict;
    std::string name;
    Texts texts;
  };

  bool operator==(const CrossReferences&, const CrossReferences&);
  std::istream& operator>>(std::istream&, CrossReferences&);
  std::ostream& operator<<(std::ostream&, const CrossReferences&);

  std::string convertWordRefToString(const WordAndTexts&);
  std::string convertTextRefToString(const TextAndLinesList&);

  void correctWord(std::string&);
}

#endif
