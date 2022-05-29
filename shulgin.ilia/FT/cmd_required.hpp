#ifndef CMD_REQUIRED_HPP
#define CMD_REQUIRED_HPP

#include <vector>
#include <map>
#include <ostream>

namespace shulgin
{
  struct InputDelimiter
  {
    char exp;
  };
  std::istream& operator>>(std::istream& in, InputDelimiter&& dest);

  bool isNumber(const std::string&);
  bool checkStreamOn(std::istream&, char);
  void printMenu(std::ostream&);

  using Words = std::vector< std::string >;
  using Lines = std::vector< size_t >;
  using TextWordsDict = std::map< std::string, Words >;
  using TextLinesDict = std::map< std::string, Lines >;
  using TextAndWords = TextWordsDict::value_type;
  using TextAndLines = TextLinesDict::value_type;

  std::ostream& operator<<(std::ostream&, const TextWordsDict&);
  std::ostream& operator<<(std::ostream&, const TextLinesDict&);
  std::ostream& operator<<(std::ostream&, const Words&);
  std::ostream& operator<<(std::ostream&, const Lines&);

  std::string convertWordsAndTextToString(const TextAndWords&);
  std::string convertLinesAndTextToString(const TextAndLines&);
}

#endif
