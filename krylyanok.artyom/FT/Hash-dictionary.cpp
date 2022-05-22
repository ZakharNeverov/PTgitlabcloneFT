#include "hash-dictionary.hpp"
#include <iostream>
#include <forward_list>
#include <string>
#include <iomanip>
#include <cmath>

namespace krylyanok
{
  Dictionary::Dictionary(size_t size, std::string dictName, size_t wordCounter):
    length_(makePrime(size)),
    name_(dictName),
    wordCounter_(wordCounter)
  {
    data_ = new std::forward_list< HashStruct >[length_];
  }

  Dictionary::~Dictionary()
  {
    if (data_ == nullptr)
    {
      return;
    }
    delete[]data_;
  }

  Dictionary::Dictionary(Dictionary&& src) noexcept:
    data_(src.data_),
    length_(src.length_),
    name_(src.name_),
    wordCounter_(src.wordCounter_)
  {
    src.data_ = nullptr;
    src.length_ = 0;
    src.name_.clear();
    src.wordCounter_ = 0;
  }

  Dictionary& Dictionary::operator=(Dictionary&& src) noexcept
  {
    std::swap(data_, src.data_);
    std::swap(length_, src.length_);
    std::swap(name_, src.name_);
    std::swap(wordCounter_, src.wordCounter_);
    return *this;
  }

  void Dictionary::add(std::string& word)
  {
    wordCounter_++;
    size_t hashIndex = hashFunc(word, length_);
    if (!data_[hashIndex].empty())
    {
      size_t size = 0;
      for (auto it = data_[hashIndex].begin(); it != data_[hashIndex].end(); it++)
      {
        size++;
        if (it->word_ == word)
        {
          it->freq_++;
          return;
        }
      }
      auto insertIt = data_[hashIndex].begin();
      for (size_t i = 0; i < size - 1; i++)
      {
        insertIt++;
      }
      data_[hashIndex].insert_after(insertIt, { word });
      return;
    }
    data_[hashIndex].push_front({ word });
  }

  void Dictionary::searhWord(std::ostream& out, const std::string& word) const
  {
    size_t hashIndex = hashFunc(word, length_);

    if (data_[hashIndex].empty())
    {
      out << 0 << "\n";
      return;
    }

    for (HashStruct& el : data_[hashIndex])
    {
      if (el.word_ == word)
      {
        out << el.freq_ << "\n";
        return;
      }
    }
    out << 0 << "\n";
    return;
  }

  bool Dictionary::deleteWord(const std::string& word) const
  {
    size_t hashIndex = hashFunc(word, length_);
    if (data_[hashIndex].empty())
    {
      return false;
    }

    auto it = data_[hashIndex].begin();

    if (it->word_ == word)
    {
      data_[hashIndex].pop_front();
      return true;
    }

    for (auto preIt = it++; it != data_[hashIndex].end(); it++, preIt++)
    {
      if (it->word_ == word)
      {
        data_[hashIndex].erase_after(preIt);
        return true;
      }
    }
    return false;
  }

  size_t Dictionary::WordFreq(const std::string& word) const
  {
    size_t hashIndex = hashFunc(word, length_);

    if (data_[hashIndex].empty())
    {
      return 0;
    }

    for (HashStruct& el : data_[hashIndex])
    {
      if (el.word_ == word)
      {
        return el.freq_;
      }
    }
    return 0;
  }

  void Dictionary::printTop(std::ostream& out) const
  {
    std::vector< std::string >  topWord;
    size_t topFreq = 0;
    for (int i = 0; i < length_; ++i)
    {
      if (!data_[i].empty())
      {
        for (HashStruct& el : data_[i])
        {
          if (el.freq_ > topFreq)
          {
            topWord.clear();
            topWord.push_back(el.word_);
            topFreq = el.freq_;
          }
          else if (el.freq_ == topFreq)
          {
            topWord.push_back(el.word_);
          }
        }
      }
    }
    for (std::string el : topWord)
    {
      out << el << std::setw(8) << topFreq << "\n";
    }
  }

  void Dictionary::printDictionary(std::ostream& out) const
  {
    out << "\nprinting table..... \n";
    size_t nomber = 1;
    for (int i = 0; i < length_; ++i)
    {
      if (!data_[i].empty())
      {
        for (HashStruct& el : data_[i])
        {
          out << std::setw(4) << nomber++ << " ";
          int setw = 15;
          out << std::setw(setw) << el.word_ << std::setw(10) << round(el.freq_ / static_cast<double>(wordCounter_) * 1000) / 1000 << "\n";
          setw = 20;
        }
      }
    }
  }

  size_t Dictionary::makePrime(const size_t len) const
  {
    if (len < 1)
    {
      throw std::invalid_argument("EMPTY LENGTH TO CREATE DICTIONARY");
    }
    size_t primeNum = len;
    for (int i = 2; i < std::sqrt(primeNum); ++i)
    {
      if (primeNum % i == 0)
      {
        primeNum++;
        i = 2;
      }
    }
    return primeNum;
  }

  size_t Dictionary::hashFunc(const std::string& str, int size) const
  {
    std::hash<std::string> temp;
    return temp(str) % length_;
  }

  size_t Dictionary::amountWordsWithFreq(const size_t freq) const
  {
    if (freq == 0)
    {
      return 0;
    }
    size_t counter = 0;
    for (size_t i = 0; i < length_; ++i)
    {
      if (!data_[i].empty())
      {
        for (HashStruct el : data_[i])
        {
          if (el.freq_ == freq)
          {
            counter++;
          }
        }
      }
    }
    return counter;
  }

  std::string Dictionary::getName()
  {
    return name_;
  }

  Dictionary mergeDict(const Dictionary& left, const Dictionary& right, std::string& dictName)
  {
    size_t temphash;
    int length = left.makePrime(left.length_ + right.length_);
    std::forward_list< Dictionary::HashStruct >* temparr = new std::forward_list< Dictionary::HashStruct >[length];
    for (int i = 0; i < left.length_; ++i)
    {
      if (!left.data_[i].empty())
      {
        for (Dictionary::HashStruct& el : left.data_[i])
        {
          temphash = left.hashFunc(el.word_, length);
          if (!temparr[temphash].empty())
          {
            size_t size = 0;
            for (auto it = temparr[temphash].begin(); it != temparr[temphash].end(); it++)
            {
              size++;
            }
            auto insertIt = temparr[temphash].begin();
            for (size_t i = 0; i < size - 1; i++)
            {
              insertIt++;
            }
            temparr[temphash].insert_after(insertIt, Dictionary::HashStruct(el.word_, el.freq_));
            continue;
          }
          temparr[temphash].push_front(Dictionary::HashStruct(el.word_, el.freq_));
        }
      }
    }
    for (int i = 0; i < right.length_; ++i)
    {
      bool flag = 0;
      if (!right.data_[i].empty())
      {
        for (Dictionary::HashStruct& el : right.data_[i])
        {
          temphash = right.hashFunc(el.word_, length);
          if (!temparr[temphash].empty())
          {
            size_t size = 0;
            for (auto it = temparr[temphash].begin(); it != temparr[temphash].end(); it++)
            {
              size++;
              if (it->word_ == el.word_)
              {
                it->freq_ += el.freq_;
                flag = 1;
                break;
              }
            }
            if (flag)
              continue;
            auto insertIt = temparr[temphash].begin();
            for (size_t i = 0; i < size - 1; i++)
            {
              insertIt++;
            }
            temparr[temphash].insert_after(insertIt, Dictionary::HashStruct(el.word_, el.freq_));
            continue;
          }
          temparr[temphash].push_front(Dictionary::HashStruct(el.word_, el.freq_));
        }
      }

    }
    Dictionary temparrr(left.length_ + right.length_, dictName, left.wordCounter_ + right.wordCounter_);
    delete[] temparrr.data_;
    temparrr.data_ = temparr;
    return temparrr;
  }
}
