#include "StreamProcessing.h"
#include <iostream>
#include <list>
#include <string>

namespace dudnik
{
  std::list< std::string > getWords(std::istream& in, char delim)
  {
    auto words = std::list< std::string >();
    char next = in.get();
    while ((next != delim) && !in.eof())
    {
      std::string elem;
      while (!std::isspace(next) && (next != delim) && !in.eof())
      {
        elem.push_back(next);
        next = in.get();
      }
      words.push_back(elem);
      next = (next == delim) ? next : in.get();
    }
    return words;
  }
  Dict getDictionary(const std::list< std::string >& input)
  {
    Dict returnable;
    int key = 0;
    std::string value;
    for (auto i = (++input.cbegin()); i != input.cend(); ++i)
    {
      if (std::isdigit(i->front()) || (i->front() == '-'))
      {
        key = std::stoi(*i);
        continue;
      }
      value = *i;
      returnable.emplace(key, value);
    }
    return returnable;
  }
  AllDicts getDictsFromStream(std::istream& in)
  {
    if (!in.good())
    {
      throw std::logic_error("Unable to read from the stream");
    }
    AllDicts dicts;
    while (!in.eof())
    {
      auto words = getWords(in, '\n');
      if (words.empty())
      {
        continue;
      }
      auto elem = getDictionary(words);
      dicts.emplace(words.front(), elem);
    }
    return dicts;
  }
}
