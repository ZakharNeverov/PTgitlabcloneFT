#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>

namespace roletskaya
{
  using map = std::map< std::string, std::list< std::string > >;
  class Dictionary
  {
  private:
    map dictionary_;

    bool checkWord(const std::string& string) const;
    bool checkTranslation(const std::string& string) const;
    bool insertTranslation(std::list< std::string >& list, std::string& line);

  public:
    Dictionary();
    Dictionary(const Dictionary& src) = default;
    Dictionary(Dictionary&& src) noexcept;
    Dictionary& operator=(const Dictionary& src) = default;
    Dictionary& operator=(Dictionary&& src) noexcept;
    Dictionary& operator=(map&& src) noexcept;
    ~Dictionary() = default;

    bool createDictionary(std::istream& in);
    bool insert(const std::string& string);
    bool findWordByTranslation(const std::string& trnsl, std::ostream& out);
    bool findTranslation(const std::string& word, std::ostream& out);
    bool deleteKey(const std::string& word);
    void print(std::ostream& out) const;
    void translateText(const std::string& string, std::ostream& out);

    friend map merge(std::vector< Dictionary >& dictsVector);
    friend void complement(std::vector< Dictionary >& dictsVector);
    friend bool equals(std::vector< Dictionary >& dictsVector);
  };
}
#endif
