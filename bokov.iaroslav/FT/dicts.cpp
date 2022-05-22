#include "dicts.hpp"
#include <fstream>
#include <ostream>
#include <stdexcept>

void bokov::makeDictionary(const std::string& fileName, dictionary& dict)
{
  std::ifstream fin(fileName);
  if (!fin.is_open())
  {
    throw std::invalid_argument("File not open!");
  }
  std::string word;
  std::string translation;
  while (fin >> word >> translation)
  {
    dict[word].insert(translation);
  }
  fin.close();
}