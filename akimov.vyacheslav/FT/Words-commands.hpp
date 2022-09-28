#ifndef WORDS_COMMANDS_HPP
#define WORDS_COMMANDS_HPP
#include <string>
namespace akimov
{
  std::string getWord(std::string& text);
  bool isWord(const std::string& word);
  bool isNum(const std::string& word);
  bool isLetter(char symbol);
  bool isLowercaseLetter(char symbol);
  bool hasNum(const std::string& word);
  bool isGoodDash(const std::string& word);
  bool isCorrect(const std::string& word);
  void skipSpaces(std::string& word);
  void erasePuncts(std::string& word);
}
#endif
