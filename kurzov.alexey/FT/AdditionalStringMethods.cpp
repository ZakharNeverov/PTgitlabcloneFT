#include "AdditionalStringMethods.hpp"

std::string kurzov::getNextWord(std::string& str, char delimeter)
{
  size_t next_space = str.find_first_of(delimeter);
  std::string word = str.substr(0, next_space);
  str.erase(0, ((next_space == str.npos) ? next_space : next_space + 1));
  return word;
}

std::string kurzov::getNameOfFile(std::string str)
{
  return str.substr(0, str.find_first_of('.'));
}
