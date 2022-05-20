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
    std::map< std::string, std::list< std::string > > dictionary_;

    bool checkWord(const std::string& string) const;
    bool checkTranslation(const std::string& string) const;

  public:
    Dictionary();
    Dictionary(const Dictionary& src) = delete;
    Dictionary(Dictionary&& src) noexcept;
    Dictionary& operator=(const Dictionary& src) = delete;
    Dictionary& operator=(Dictionary&& src) noexcept;
    ~Dictionary() = default;

    void createDictionary(std::istream& in);
    bool insert(const std::string& string);
    bool findWordByTranslation(const std::string& trnsl, std::ostream& out);
    bool findTranslation(const std::string& word, std::ostream& out);
    bool deleteKey(const std::string& word);
    void print(std::ostream& out) const;
    void translateText(std::istream& in, std::ostream& out);

    friend map merge(std::vector< map >& dictsVector);
    friend map complement(std::vector< map >& dictsVector);
    friend bool equals(std::vector< map >& dictsVector);

    friend std::vector< map > getFiles(std::string& files);
    void inputResult(map dict);
  };
}
#endif
