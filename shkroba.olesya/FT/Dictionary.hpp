#ifndef RGR_ENG_RUS_DICTIONARY_HPP
#define RGR_ENG_RUS_DICTIONARY_HPP
#include <map>
#include <set>
#include <memory>
//#include <ostream>
#include <string>
#include <iosfwd>
namespace shkroba
{
  using setPointer = std::shared_ptr< std::set< std::string > >;
  using pairER = std::pair< std::string, setPointer >;

  class Dictionary
  {
  public:
    Dictionary() = default;
    ~Dictionary() = default;
    explicit Dictionary(const std::string& name);
    Dictionary& operator=(const Dictionary& dictionary) = default;
    Dictionary& operator=(Dictionary&& dictionary) noexcept;
    Dictionary(const Dictionary& dictionary) = default;
    Dictionary(Dictionary&& dictionary) noexcept = default;
    void insert(const std::string& word, const std::string& translate);
    void insert(const pairER& item);

    std::map< std::string, setPointer >::const_iterator
    search(const std::string& word) const;
    std::string getName() const;
    void rename(const std::string& newName);
    bool deleteWord(const std::string& word);
    void printDictionary(std::ostream& out) const;
    size_t size() const;
    void findWord(std::string letter, std::ostream& out) const;
    void addWords(const Dictionary& dictionary);

    std::map< std::string, setPointer >::const_iterator begin() const;
    std::map< std::string, setPointer >::iterator begin();
    std::map< std::string, setPointer >::const_iterator end() const;
    std::map< std::string, setPointer >::iterator end();

    friend std::istream& operator>>(std::istream& in, Dictionary& dictionary);

    bool operator==(const Dictionary& rhs) const;
    bool operator!=(const Dictionary& rhs) const;

    static Dictionary createFromUniqueWords(const Dictionary& d1, const Dictionary& d2);
    static void commonForTwo(const Dictionary& source, const Dictionary& extra, std::ostream& out);
    static Dictionary createFromOneTranslate(const Dictionary& dictionary);
  private:
    std::map< std::string, setPointer > dict_;
    std::string name_;
  };
}

#endif
