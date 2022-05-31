#ifndef RGR_ENG_RUS_DICTIONARY_HPP
#define RGR_ENG_RUS_DICTIONARY_HPP
#include <map>
#include <set>
#include <memory>
#include <iosfwd>

namespace shkroba
{
  using pairER = std::pair< std::string, std::shared_ptr< std::set< std::string > > >;

  class Dictionary
  {
  public:
    Dictionary() = default;
    ~Dictionary() = default;
    explicit Dictionary(const std::string& name);
    Dictionary &operator=(const Dictionary& dictionary) = default;
    Dictionary &operator=(Dictionary&& dictionary) noexcept;
    Dictionary(const Dictionary& dictionary) = default;
    Dictionary(Dictionary&& dictionary) noexcept = default;
    void insert(const std::string& word, const std::string& translate);
    void insert(const pairER& item);

    std::map< std::string, std::shared_ptr< std::set< std::string > > >::const_iterator
    search(const std::string& word) const;
    std::string getName() const;
    void rename(const std::string& newName);
    bool deleteWord(const std::string& word);
    void printDictionary(std::ostream& out) const;
    size_t size() const;
    void findWord(char letter, std::ostream& out) const;
    void addWords(const Dictionary& dictionary);

    std::map< std::string, std::shared_ptr< std::set< std::string > > >::const_iterator begin() const;
    std::map< std::string, std::shared_ptr< std::set< std::string > > >::iterator begin();
    std::map< std::string, std::shared_ptr< std::set< std::string > > >::const_iterator end() const;
    std::map< std::string, std::shared_ptr< std::set< std::string > > >::iterator end();

    friend std::istream &operator>>(std::istream& in, Dictionary& dictionary);

    bool operator==(const Dictionary& rhs) const;
    bool operator!=(const Dictionary& rhs) const;

    std::map< std::string, std::shared_ptr< std::set< std::string > > >& getDictionary();
    const std::map< std::string, std::shared_ptr< std::set< std::string > > >& getDictionary() const;


  private:
    std::map< std::string, std::shared_ptr< std::set< std::string > > > dictionary_;
    std::string name_;
  };
}

#endif
