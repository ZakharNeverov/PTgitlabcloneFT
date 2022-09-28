#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <string>
#include <iosfwd>
#include <iterator>
#include <map>
namespace akimov
{
  using cnstStr = const std::string;
  using pairSS = std::pair< std::string, std::size_t >;
  using mapIter = std::map< std::string, std::size_t >::iterator;
  using mapCosntIter = std::map< std::string, std::size_t >::const_iterator;
  class Dictionary
  {
  public:
    Dictionary() = default;
    Dictionary(const Dictionary& other) = default;
    Dictionary(cnstStr& name);
    ~Dictionary() = default;
    bool operator==(const Dictionary& dict) const;

    std::string getName() const;
    std::size_t getSum() const;
    inline bool empty() const;
    void rename(cnstStr& name);
    void print(std::ostream& stream) const;
    void printPair(std::ostream& stream, const pairSS& pair) const;
    void printMostPopular(std::ostream& stream) const;

    void insert(cnstStr& word);
    void insertPair(const pairSS pair);
    mapCosntIter search(cnstStr& word) const;
    mapIter search(cnstStr& word);
    void subtractPair(const pairSS pair);
    bool deleteWord(cnstStr& word);
    void drop();

    mapCosntIter begin() const;
    mapCosntIter end() const;
    mapIter begin();
    mapIter end();

  private:
    std::string name_;
    std::map< std::string, std::size_t > dictionary_;
  };
}
#endif
