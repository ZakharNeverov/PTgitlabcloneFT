#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <fstream>
#include <string>
#include <unordered_map>

namespace surby
{
  using textes_map = std::unordered_map < std::string, std::string >;

  void addTextCommand(textes_map&, std::string&, std::istream& in);
  void getDictionaryCommand(textes_map&, std::string&, std::ostream& out);
  void getTextCommand(textes_map&, std::string&, std::ostream& out);
  void encodeCommand(textes_map&, std::string&);
  void decodeCommand(textes_map&, std::string&);
  void encodeKeyCommand(textes_map&, std::string&);
  void checkDifferenceCommand(textes_map&, std::string&, std::ostream&);
  void printDifference(int, int, std::ostream&);
  void isTextExist(const std::string& name, textes_map& texts, std::ostream& out);
}
#endif
