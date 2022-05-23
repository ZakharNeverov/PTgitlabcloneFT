#ifndef SHANNON_FANO_HPP
#define SHANNON_FANO_HPP

#include <string>
#include <vector>
#include <map>

namespace gusarov {
  struct CodeFano_t
  {
    char symbol;
    size_t freq;
    std::string code;
  };
  using fanoAlphabet_t = std::vector< CodeFano_t >;
  using dict_t = std::map< char, std::string >;
  using reverseDict_t = std::map< std::string, char >;

  dict_t getCode(const std::string&);
  std::string getEncodedText(const std::string&, dict_t);
  std::string getDecodedText(const std::string&, dict_t);
  size_t getSize(const std::string&);

  fanoAlphabet_t getFanoAlphobet(const std::string&);
  void shannon(size_t l, size_t h, fanoAlphabet_t&);
  fanoAlphabet_t getFrequency(const std::string&);
  reverseDict_t getReverseDictionary(dict_t);
}
#endif
