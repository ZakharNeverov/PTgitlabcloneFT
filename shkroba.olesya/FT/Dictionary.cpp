#include "Dictionary.hpp"
#include <iterator>
#include "Utilities.hpp"
#include <iostream>


namespace shkroba
{
  Dictionary::Dictionary(const std::string& name) :
    name_(name)
  {}

  void Dictionary::insert(const std::string& word, const std::string& translate)
  {
    auto iterator = dictionary_.find(word);
    if (iterator != dictionary_.end())
    {
      iterator->second->insert(translate);
    }
    else
    {
      std::set< std::string > set;
      std::shared_ptr< std::set< std::string > > ptr = std::make_shared< std::set< std::string > >(set);
      ptr->insert(translate);
      pairER pair = std::make_pair(word, ptr);
      dictionary_.insert(pair);
    }
  }

  void Dictionary::insert(const pairER& item)
  {
    for (const auto& item1: *item.second)
    {
      this->insert(item.first, item1);
    }
  }

  std::map< std::string, std::shared_ptr< std::set< std::string > > >::const_iterator
  Dictionary::search(const std::string& word) const
  {
    return dictionary_.find(word);
  }

  bool Dictionary::deleteWord(const std::string& word)
  {
    auto iter = dictionary_.find(word);
    if (iter != dictionary_.end())
    {
      dictionary_.erase(word);
      return true;
    }
    return false;
  }

  void Dictionary::printDictionary(std::ostream& out) const
  {
    out << name_ << '\n';
    for (auto& item: dictionary_)
    {
      out << item.first << ' ' << '-' << ' ' << *item.second;
    }
  }

  size_t Dictionary::size() const
  {
    return dictionary_.size();
  }

  void Dictionary::findWord(char letter, std::ostream& out) const
  {
    std::vector< std::string > result;
    for (auto& item: dictionary_)
    {
      if (item.first[0] == letter)
      {
        result.push_back(item.first);
      }
    }
    for (auto& iter: result)
    {
      out << iter << ' ';
    }
    if (result.empty())
    {
      out << "No words beginning with this letter." << "\n";
    }
  }

  void Dictionary::addWords(const Dictionary& dictionary)
  {
    for (auto& word: dictionary.dictionary_)
    {
      for (const auto& translate: *word.second)
      {
        this->insert(word.first, translate);
      }
    }
  }

  std::string Dictionary::getName() const
  {
    return name_;
  }

  std::map< std::string, std::shared_ptr< std::set< std::string > > >::const_iterator Dictionary::begin() const
  {
    return dictionary_.begin();
  }

  const std::map< std::string, std::shared_ptr< std::set< std::string > > >& Dictionary::getDictionary() const
  {
    return dictionary_;
  }

  std::map< std::string, std::shared_ptr< std::set< std::string > > >::const_iterator Dictionary::end() const
  {
    return dictionary_.end();
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
      std::shared_ptr< std::set< std::string > > ptr = std::make_shared< std::set< std::string > >(set);
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
    return this->dictionary_ == rhs.dictionary_;
  }

  bool Dictionary::operator!=(const Dictionary& rhs) const
  {
    return !(*this == rhs);
  }

  std::map< std::string, std::shared_ptr< std::set< std::string> > >::iterator Dictionary::begin()
  {
    return dictionary_.begin();
  }

  std::map< std::string, std::shared_ptr< std::set< std::string > > >::iterator Dictionary::end()
  {
    return dictionary_.end();
  }

  Dictionary& Dictionary::operator=(Dictionary&& dictionary) noexcept
  {
    dictionary_ = std::move(dictionary.dictionary_);
    name_ = std::move(dictionary.name_);
    return *this;
  }

  std::map< std::string, std::shared_ptr< std::set< std::string > > >& Dictionary::getDictionary()
  {
    return dictionary_;
  }
};
