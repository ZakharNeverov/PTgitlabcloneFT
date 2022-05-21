#ifndef FREQUENCYDICTIONARY_HASH_DICTIONARY_HPP
#define FREQUENCYDICTIONARY_HASH_DICTIONARY_HPP

#include <iostream>
#include <fstream>
#include <forward_list>
#include <string>
#include <vector>

namespace krylyanok
{
  class Dictionary
  {
  public:
    friend Dictionary mergeDict(const Dictionary& left, const Dictionary& right, std::string& dictName);
    Dictionary() = delete;
    Dictionary(size_t size, std::string dictName, size_t wordCounter = 0);
    Dictionary(const Dictionary& src) = delete;
    Dictionary(Dictionary&& src) noexcept;
    Dictionary& operator=(const Dictionary& src) = delete;
    Dictionary& operator=(Dictionary&& src) noexcept;
    ~Dictionary();
    void add(std::string& word);
    void searhWord(std::ostream& out, const std::string& word) const;
    bool deleteWord(const std::string& word) const;
    size_t WordFreq(const std::string& word) const;
    void printTop(std::ostream& out) const;
    void printDictionary(std::ostream& out) const;
    size_t amountWordsWithFreq(size_t freq) const;
    std::string getName();

  private:
    struct HashStruct
    {
      int freq_;
      std::string word_;
      HashStruct(std::string& word):
        freq_(1),
        word_(word)
      {}
      HashStruct(std::string& word, int& freq):
        freq_(freq),
        word_(word)
      {}
      HashStruct() :
        freq_(0),
        word_("initialization")
      {}
    };
    std::forward_list< HashStruct >* data_;
    int length_;
    std::string name_;
    size_t wordCounter_;
    size_t makePrime(size_t len) const;
    size_t hashFunc(const std::string& str, int size) const;
  };
}
#endif
