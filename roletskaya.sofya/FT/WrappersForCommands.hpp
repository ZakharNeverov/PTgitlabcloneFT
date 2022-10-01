#ifndef WRAPPER_FOR_COMMANDS_HPP
#define WRAPPER_FOR_COMMANDS_HPP
#include "Dictionary.hpp"

using dictsArray = std::map< std::string, roletskaya::Dictionary >;

namespace roletskaya
{
  void insert(std::string& line, dictsArray& dictsArray, std::ofstream& out);
  void findWordByTranslation(std::string& line, dictsArray& dictsArray, std::ofstream& out);
  void findTranslation(std::string& line, dictsArray& dictsArray, std::ofstream& out);
  void deleteKey(std::string& line, dictsArray& dictsArray, std::ofstream& out);
  void print(std::string& fileName, dictsArray& dictsArray, std::ofstream& out);
  void translateText(std::string& line, dictsArray& dictsArray, std::ofstream& out);
  void doMerge(std::string& line, dictsArray& dictsArray, std::ofstream& out);
  void doComplement(std::string& line, dictsArray& dictsArray, std::ofstream& out);
  void ifEquals(std::string& line, dictsArray& dictsArray, std::ofstream& out);
}
#endif
