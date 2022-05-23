#include "io-utils.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "dictionary_utils.h"

rakitin::dictionary rakitin::getDictionary(const std::string& path)
{
  dictionary result;
  std::ifstream stream;
  stream.open(path);
  if (!stream.is_open()) {
    throw std::logic_error("Can't open file " + path);
  }
  int line = 1;
  while (!stream.eof()) {
    std::string str;
    stream >> str;
    rakitin::addWord(result, str, line);
    char c = stream.get();
    if (c == '\n') {
      line++;
    }
  }
  stream.close();

  return result;
}
