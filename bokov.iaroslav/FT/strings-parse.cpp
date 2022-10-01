#include "strings-parse.hpp"

std::string bokov::getSubString(std::string& str)
{
  size_t pos = str.find_first_of(' ', 0);
  std::string s = str.substr(0, pos);
  pos = (pos == str.npos) ? pos : pos + 1;
  str.erase(0, pos);
  return s;
}

std::vector < std::string > bokov::getRowOfWords(std::string& str)
{
  std::vector < std::string > vect;
  while (!str.empty())
  {
    vect.push_back(bokov::getSubString(str));
  }
  return vect;
}
