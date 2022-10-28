#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include "reading.hpp"

namespace bazzhin
{
  using dictionaries = std::map< std::string, dictionary >;

  struct Commands
  {
    Commands(std::istream& in, std::ostream& out, dictionaries& dicts);
    void createDictionary();
    void deleteDictionary();
    void printDictionary();
    void searchWordInDictionary();
    void deleteWordInDictionary();
    void deleteWordInAllDictionaries();
    void printAllWords();
    void addWordsFromText();
    void printPeriodOfWord();
    void insertWord();
    void printMainWords();

  private:
    std::istream& in_;
    std::ostream& out_;
    dictionaries& dicts_;
    dictionary createCommonDictionary();
  };
}
#endif
