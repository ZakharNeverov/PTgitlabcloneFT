#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <string>
#include <unordered_map>

namespace krylyanok
{
  struct HashStruct
  {
    int freq_;
    std::string word_;
    HashStruct() = delete;
    HashStruct(std::string& word):
      freq_(1),
      word_(word)
    {}
    HashStruct(std::string& word, int& freq):
      freq_(freq),
      word_(word)
    {}
  };

  struct NameMap
  {
    std::string name_;
    std::unordered_map < std::string, HashStruct > dict_;
    size_t wordCounter_;
    NameMap() = delete;
    NameMap(std::string& word):
      name_(word),
      dict_(),
      wordCounter_(0)
    {}
  };
}
#endif
