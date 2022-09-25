#include "Dictionary.hpp"
#include <iterator>
#include <iostream>
#include <algorithm>
#include <functional>
#include "Utilities.hpp"

namespace shkroba
{
  using namespace std::placeholders;
  Dictionary::Dictionary(const std::string& name):
    name_(name)
  {}

  void Dictionary::insert(const std::string& word, const std::string& translate)
  {
    auto iterator = dict_.find(word);
    if (iterator != dict_.end())
    {
      iterator->second->insert(translate);
    }
    else
    {
      std::set< std::string > set;
      setPointer ptr = std::make_shared< std::set< std::string > >(set);
      ptr->insert(translate);
      pairER pair = std::make_pair(word, ptr);
      dict_.insert(pair);
    }
  }

  void Dictionary::insert(const pairER& item)
  {
    dict_.insert(item);
  }

  std::map< std::string, setPointer >::const_iterator Dictionary::search(const std::string& word) const
  {
    return dict_.find(word);
  }

  bool Dictionary::deleteWord(const std::string& word)
  {
    auto iter = dict_.find(word);
    if (iter != dict_.end())
    {
      dict_.erase(word);
      return true;
    }
    return false;
  }

  void Dictionary::printDictionary(std::ostream& out) const
  {
    out << name_ << '\n';
    for (auto& item: dict_)
    {
      out << item.first << ' ' << '-' << ' ' << *item.second;
    }
  }

  size_t Dictionary::size() const
  {
    return dict_.size();
  }

  bool isSame(const pairER& word, const std::string& letter)
  {
    return letter[0] == word.first[0];
  }

  void Dictionary::findWord(std::string letter, std::ostream& out) const
  {
    std::vector< pairER > result;
    std::copy_if(dict_.begin(), dict_.end(), std::back_inserter(result), std::bind(isSame, _1, letter));
    for (auto& item: result)
    {
      out << item.first << ' ' << '-' << ' ' << *item.second;;
    }
    if (result.empty())
    {
      out << "No words beginning with this letter." << "\n";
    }
  }

  void Dictionary::addWords(const Dictionary& dictionary)
  {
    for (auto& word: dictionary.dict_)
    {
      this->insert(word);
    }
  }

  std::string Dictionary::getName() const
  {
    return name_;
  }

  std::map< std::string, setPointer >::const_iterator Dictionary::begin() const
  {
    return dict_.begin();
  }

  std::map< std::string, setPointer >::const_iterator Dictionary::end() const
  {
    return dict_.end();
  }

  std::istream& operator>>(std::istream& in, Dictionary& dictionary)
  {
    std::string newLine;
    std::getline(in, newLine, '\n');
    std::string name = nextWord(newLine);
    dictionary.name_ = name;
    std::string word;
    std::set< std::string > set;
    while (!in.eof())
    {
      std::getline(in, newLine, '\n');
      word = nextWord(newLine);
      if (word.empty())
      {
        return in;
      }
      while (!newLine.empty())
      {
        std::string translate = nextWord(newLine);
        set.insert(translate);
      }
      setPointer ptr = std::make_shared< std::set< std::string > >(set);
      pairER pair = std::make_pair(word, ptr);
      dictionary.insert(pair);
      set.clear();
    }
    return in;
  }

  void Dictionary::rename(const std::string& newName)
  {
    name_ = newName;
  }

  bool Dictionary::operator==(const Dictionary& rhs) const
  {
    return this->dict_ == rhs.dict_;
  }

  bool Dictionary::operator!=(const Dictionary& rhs) const
  {
    return !(*this == rhs);
  }

  std::map< std::string, setPointer >::iterator Dictionary::begin()
  {
    return dict_.begin();
  }

  std::map< std::string, setPointer >::iterator Dictionary::end()
  {
    return dict_.end();
  }

  Dictionary& Dictionary::operator=(Dictionary&& dictionary) noexcept
  {
    dict_ = std::move(dictionary.dict_);
    name_ = std::move(dictionary.name_);
    return *this;
  }

  Dictionary Dictionary::createFromUniqueWords(const Dictionary& d1, const Dictionary& d2)
  {
    Dictionary common("common");
    std::set_intersection(
      d1.begin(),
      d1.end(),
      d2.begin(),
      d2.end(),
      std::inserter(common.dict_, common.begin())
    );
    return common;
  }

  void Dictionary::commonForTwo(const Dictionary& source, const Dictionary& extra, std::ostream& out)
  {
    Dictionary result;
    std::merge(source.begin(),
               source.end(),
               extra.begin(),
               extra.end(),
               std::inserter(result.dict_, result.begin())
    );
    result.addWords(source);
    result.addWords(extra);
    result.printDictionary(out);
  }

  Dictionary Dictionary::createFromOneTranslate(const Dictionary& dictionary)
  {
    std::cout << "Input name of new dictionary" << '\n';
    std::string name;
    std::cin >> name;
    Dictionary newDictionary(name);
    std::copy_if(
      dictionary.begin(),
      dictionary.end(),
      std::inserter(
            newDictionary.dict_,
            newDictionary.begin()),
      [](const std::pair< std::string, std::shared_ptr< std::set< std::string > > >& pair)
      {
        return pair.second->size() == 1;
      }
    );
    return newDictionary;
  }
};
