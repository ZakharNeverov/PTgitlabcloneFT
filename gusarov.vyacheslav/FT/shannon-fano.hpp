#ifndef SHENNON_FANO_HPP
#define SHENNON_FANO_HPP

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

  fanoAlphabet_t getFanoAlphobet(std::ifstream& file);
  void shannon(size_t l, size_t h, fanoAlphabet_t& fanoAlphabet);
  fanoAlphabet_t getFrequencyForCode(std::ifstream& file);
  dict_t getDictionary(std::ifstream& fileCode);
  reverseDict_t getReverseDictionary(std::ifstream& fileCode);

  void doEncode(std::ifstream& in, std::ofstream& out, dict_t fano);
  void doDecode(std::ifstream& in, std::ofstream& out, reverseDict_t fano);
  void createCodeFano(std::ifstream& in, std::ofstream& out);
}
#endif
