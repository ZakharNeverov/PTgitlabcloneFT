#include "operations.hpp"
#include <iomanip>
#include "commands.hpp"
#include "dictionary.hpp"
#include "operations.hpp"
#include <unordered_set>

namespace
{
  using map = std::unordered_map <std::string, krylyanok::HashStruct >;

  size_t findElem(std::string& dictName, std::vector< krylyanok::NameMap >& dicts)
  {
    size_t i = 0;
    for (i; i < dicts.size(); i++)
    {
      if (dicts[i].name_ == dictName)
      {
        break;
      }
      if (dicts.size() - 1 == i)
      {
        return -1;
      }
    }
    return i;
  }

  void printDictionary(std::ostream& out, const krylyanok::NameMap& temp)
  {
    size_t nomber = 1;
    for (size_t i = 0; i < temp.dict_.bucket_count(); i++)
    {
      if (temp.dict_.cbegin(i) == temp.dict_.cend(i))
      {
        continue;
      }
      for (auto it = temp.dict_.cbegin(i); it != temp.dict_.cend(i); ++it)
      {
        out << std::setw(4) << nomber++ << " ";
        int setw = 15;
        out << std::setw(setw) << (*it).second.word_ << std::setw(10) << round((*it).second.freq_ / static_cast<double>(temp.wordCounter_) * 1000) / 1000 << "\n";
      }
    }
  }
  void printTop(std::ostream& out, const map& temp)
  {
    std::vector< std::string >  topWord;
    size_t topFreq = 0;
    for (size_t i = 0; i < temp.bucket_count(); i++)
    {
      if (temp.cbegin(i) == temp.cend(i))
      {
        continue;
      }
      for (auto it = temp.cbegin(i); it != temp.cend(i); ++it)
      {
        if ((*it).second.freq_ > topFreq)
        {
          topWord.clear();
          topWord.push_back((*it).second.word_);
          topFreq = (*it).second.freq_;
        }
        else if ((*it).second.freq_ == topFreq)
        {
          topWord.push_back((*it).second.word_);
        }
      }
    }
    for (std::string el : topWord)
    {
      out << std::setw(10) << el << " " << std::setw(8) << topFreq << "\n";
    }
  }
  void searhWord(std::ostream& out, const std::string& word, const map& temp)
  {
    auto iter = temp.find(word);
    if (iter != (temp.end()))
    {
      out << (*iter).second.freq_ << "\n";
      return;
    }
    out << 0 << "\n";
  }

  map mergeDict(const map& left, const map& right)
  {
    map newDict;
    for (size_t i = 0; i < left.bucket_count(); i++)
    {
      if (left.cbegin(i) == left.cend(i))
      {
        continue;
      }
      for (auto it = left.cbegin(i); it != left.cend(i); ++it)
      {
        std::pair < std::string, krylyanok::HashStruct > pair((*it).first, (*it).second);
        newDict.insert(pair);
      }
    }
    for (size_t i = 0; i < right.bucket_count(); i++)
    {
      if (right.cbegin(i) == right.cend(i))
      {
        continue;
      }
      for (auto it = right.cbegin(i); it != right.cend(i); ++it)
      {
        auto iter = newDict.find((*it).first);
        if (iter != (newDict.end()))
        {
          (*iter).second.freq_ += (*it).second.freq_;
          continue;
        }
        std::pair < std::string, krylyanok::HashStruct > pair((*it).first, (*it).second);
        newDict.insert(pair);
      }
    }
    return newDict;
  }
}

void krylyanok::Commands::getLoad()
{
  std::string nameDict = "";
  std::string nameFile = "";
  in_ >> nameDict >> nameFile;
  if (!(findElem(nameDict, dictionaries_) == -1))
  {
    throw std::logic_error("");
  }
  std::vector< std::string > words;
  makeDict(nameFile, words);
  dictionaries_.push_back({nameDict});
  krylyanok::createDict(words, dictionaries_[dictionaries_.size() - 1]);
}

void krylyanok::Commands::getPrint()
{
  std::string dictName = "";
  in_ >> dictName;
  size_t i = 0;
  i = findElem(dictName, dictionaries_);
  if (i == -1)
  {
    throw std::logic_error("");
  }
  printDictionary(out_, dictionaries_[i]);
}

void krylyanok::Commands::getTop()
{
  std::string dictName = "";
  in_ >> dictName;
  size_t i = findElem(dictName, dictionaries_);
  if (i == -1)
  {
    throw std::logic_error("");
  }
  printTop(out_, dictionaries_[i].dict_);
}

void krylyanok::Commands::getSearch()
{
  std::string dictName = "";
  in_ >> dictName;
  size_t i = findElem(dictName, dictionaries_);
  if (i == -1)
  {
    throw std::logic_error("");
  }
  std::string word = "";
  in_ >> word;
  searhWord(out_, word, dictionaries_[i].dict_);
}

void krylyanok::Commands::getMerge()
{
  bool succesfulMerge = false;
  std::string mergeDictName = "";
  std::string dictName1 = "";
  in_ >> mergeDictName >> dictName1;
  if (!(findElem(mergeDictName, dictionaries_) == -1))
  {
    throw std::logic_error("");
  }
  size_t i1 = findElem(dictName1, dictionaries_);
  if (i1 == -1)
  {
    throw std::logic_error("");
  }
  NameMap mergeDicter(mergeDictName);
  while (in_.peek() != '\n')
  {
    std::string dictName2 = "";
    in_ >> dictName2;
    size_t i2 = findElem(dictName2, dictionaries_);
    if (i2 == -1)
    {
      throw std::logic_error("");
    }
    if (!succesfulMerge)
    {
      mergeDicter.dict_ = mergeDict(dictionaries_[i1].dict_, dictionaries_[i2].dict_);
      mergeDicter.wordCounter_ = dictionaries_[i1].wordCounter_ + dictionaries_[i2].wordCounter_;
      succesfulMerge = true;
      continue;
    }
    mergeDicter.dict_ = mergeDict(mergeDicter.dict_, dictionaries_[i2].dict_);
    mergeDicter.wordCounter_ += dictionaries_[i2].wordCounter_;
  }
  dictionaries_.push_back(std::move(mergeDicter));
}

void krylyanok::Commands::getDelete()
{
  std::string dictName = "";
  in_ >> dictName;
  size_t i = findElem(dictName, dictionaries_);
  if (i == -1)
  {
    throw std::logic_error("");
  }
  dictionaries_.erase(dictionaries_.begin() + i);
}
