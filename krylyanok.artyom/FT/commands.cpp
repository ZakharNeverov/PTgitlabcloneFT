#include "operations.hpp"
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include "commands.hpp"
#include "dictionary.hpp"
#include "operations.hpp"

namespace
{
  using map = std::unordered_map < std::string, krylyanok::HashStruct >;

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
        return dicts.size();
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
        double freq = round((*it).second.freq_ / static_cast<double>(temp.wordCounter_) * 1000) / 1000;
        out << std::setw(setw) << (*it).second.word_ << std::setw(10) << freq << "\n";
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

  void deleteWord(krylyanok::NameMap& temp, const std::string& word)
  {
    for (auto it = temp.dict_.begin(); it != temp.dict_.end();)
    {
      if (it->first == word)
      {
        temp.wordCounter_ -= (*it).second.freq_;
        temp.dict_.erase(it);
        return;
      }
      else
      {
        ++it;
      }
    }
  }
  
  void insertWord(krylyanok::NameMap& temp, std::string& word)
  {
    if (!krylyanok::correctString(word))
    {
      throw std::logic_error("");
    }
    for (auto it = temp.dict_.begin(); it != temp.dict_.end();)
    {
      if (it->first == word)
      {
        temp.wordCounter_++;
        (*it).second.freq_++;
        return;
      }
      else
      {
        ++it;
      }
    }
    std::pair < std::string, krylyanok::HashStruct > pair(word, { word });
    temp.dict_.insert(pair);
    temp.wordCounter_++;
  }

  void saveDictionary(std::ostream& out, const krylyanok::NameMap& temp)
  {
    out << temp.wordCounter_ << "\n";
    for (size_t i = 0; i < temp.dict_.bucket_count(); i++)
    {
      if (temp.dict_.cbegin(i) == temp.dict_.cend(i))
      {
        continue;
      }
      for (auto it = temp.dict_.cbegin(i); it != temp.dict_.cend(i); ++it)
      {
        out << (*it).second.word_ << " " << (*it).second.freq_ << "\n";
      }
    }
  }
}

void krylyanok::Commands::getLoad()
{
  std::string nameDict = "";
  std::string parameter = "";
  std::string nameFile = "";
  in_ >> parameter >> nameDict >> nameFile;
  if (!(findElem(nameDict, dictionaries_) == dictionaries_.size()))
  {
    throw std::logic_error("");
  }
  if (parameter == "TECH")
  {
    dictionaries_.push_back({ nameDict });
    makeTech(nameFile, dictionaries_[dictionaries_.size() - 1]);
    return;
  }
  else if (parameter == "TEXT")
  {
    std::vector< std::string > words;
    makeDict(nameFile, words);
    dictionaries_.push_back({ nameDict });
    krylyanok::createDict(words, dictionaries_[dictionaries_.size() - 1]);
    return;
  }
  throw std::logic_error("");
}

void krylyanok::Commands::getPrint()
{
  std::string dictName = "";
  in_ >> dictName;
  size_t i = findElem(dictName, dictionaries_);
  if (i == dictionaries_.size())
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
  if (i == dictionaries_.size())
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
  if (i == dictionaries_.size())
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
  if (!(findElem(mergeDictName, dictionaries_) == dictionaries_.size()))
  {
    throw std::logic_error("");
  }
  size_t i1 = findElem(dictName1, dictionaries_);
  if (i1 == dictionaries_.size())
  {
    throw std::logic_error("");
  }
  NameMap mergeDicter(mergeDictName);
  while (in_.peek() != '\n')
  {
    std::string dictName2 = "";
    in_ >> dictName2;
    size_t i2 = findElem(dictName2, dictionaries_);
    if (i2 == dictionaries_.size())
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
  if (i == dictionaries_.size())
  {
    throw std::logic_error("");
  }
  dictionaries_.erase(dictionaries_.begin() + i);
}

void krylyanok::Commands::getSave()
{
  std::string dictName = "";
  std::string fileName = "";
  in_ >> dictName >> fileName;
  size_t i = findElem(dictName, dictionaries_);
  if (i == dictionaries_.size())
  {
    throw std::logic_error("");
  }
  std::ofstream fout(fileName, std::ios_base::out | std::ios_base::trunc);
  if (!fout.is_open())
  {
    throw std::logic_error("");
  }
  saveDictionary(fout, dictionaries_[i]);
}

void krylyanok::Commands::getDeleteWord()
{
  std::string dictName = "";
  in_ >> dictName;
  size_t i = findElem(dictName, dictionaries_);
  if (i == dictionaries_.size())
  {
    throw std::logic_error("");
  }
  std::string word = "";
  in_ >> word;
  deleteWord(dictionaries_[i], word);
}

void krylyanok::Commands::getInsertWord()
{

  std::string dictName = "";
  in_ >> dictName;
  size_t i = findElem(dictName, dictionaries_);
  if (i == dictionaries_.size())
  {
    throw std::logic_error("");
  }
  std::string word = "";
  in_ >> word;
  insertWord(dictionaries_[i], word);
}