#ifndef RGR_ENG_RUS_DICTIONARY_HPP
#define RGR_ENG_RUS_DICTIONARY_HPP

#include <map>
#include <set>
#include <memory>
#include <ostream>

namespace shkroba
{
  using pairER = std::pair< const std::string, std::shared_ptr< std::set< std::string > > >;

  class Dictionary
  {
  public:
    Dictionary() = default;
    void insert(const std::string &word, const std::string &translate);
    void insert(const pairER &item);

    std::map< std::string, std::shared_ptr< std::set< std::string>> >::const_iterator
    search(const std::string &word) const;

    bool deleteWord(const std::string &word);
    void printDictionary(std::ostream &out);
    size_t size();
    void findWord(char letter, std::ostream& out) const;
    void addWordsFromAnother(const Dictionary &dictionary);

    std::map< std::string, std::shared_ptr< std::set< std::string > > >::const_iterator begin() const;
    std::map< std::string, std::shared_ptr< std::set< std::string > > >::const_iterator end() const;

  private:
    std::map< std::string, std::shared_ptr< std::set< std::string > > > dictionary_;
  };
}

#endif
