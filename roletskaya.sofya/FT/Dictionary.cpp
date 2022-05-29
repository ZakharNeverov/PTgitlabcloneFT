#include "Dictionary.hpp"
#include <algorithm>
#include <stdexcept>
#include "Utilites.hpp"

using map = std::map< std::string, std::list< std::string > >;

roletskaya::Dictionary::Dictionary(): dictionary_{} {}

roletskaya::Dictionary::Dictionary(Dictionary&& src) noexcept : dictionary_{std::move(src.dictionary_)} {}

roletskaya::Dictionary& roletskaya::Dictionary::operator=(Dictionary&& src) noexcept
{
  dictionary_ = std::move(src.dictionary_);
  return *this;
}

roletskaya::Dictionary& roletskaya::Dictionary::operator=(map&& src) noexcept
{
  dictionary_ = std::move(src);
  return *this;
}

bool roletskaya::Dictionary::checkWord(const std::string& string) const
{
  int fisrtLetterInASCII = 97;
  int lastLetterInASCII = 122;
  for (size_t i = 0; i < string.length(); i++)
  {
    if ((string[i] > lastLetterInASCII) || (string[i] < fisrtLetterInASCII))
    {
      return false;
    }
  }
  return true;
}

bool roletskaya::Dictionary::checkTranslation(const std::string& string) const
{
  int fisrtLetterInASCII = 224;
  int lastLetterInASCII = 255;
  for (size_t i = 0; i < string.length(); i++)
  {
    if ((string[i] > lastLetterInASCII) || (string[i] < fisrtLetterInASCII))
    {
      return false;
    }
  }
  return true;
}

bool roletskaya::Dictionary::insertTranslation(std::list< std::string >& list, std::string& line)
{
  std::string translation = "";
  bool flag = true;
  while (!line.empty())
  {
    translation = getElem(line);
    if (checkTranslation(translation) == false)
    {
      flag = false;
      continue;
    }
    list.push_back(translation);
  }
  list.sort();
  return flag;
}

bool roletskaya::Dictionary::createDictionary(std::istream& in)
{
  if (!in.good())
  {
    throw std::invalid_argument("Couldn't open the input file.\n");
    return false;
  }
  std::string line = "";
  std::string word = "";
  while (!in.eof())
  {
    std::getline(in, line);
    word = getElem(line);
    if (checkWord(word) == false)
    {
      continue;
    }
    if (line.empty())
    {
      continue;
    }
    auto item = dictionary_.find(word);
    if (item != dictionary_.end())
    {
      std::list< std::string > list;
      if (insertTranslation(list, line) == true)
      {
        if (item->second == list)
        {
          continue;
        }
        else
        {
          auto listIter = list.begin();
          while (listIter != list.end())
          {
            auto listItem = std::find(item->second.begin(), item->second.end(), *listIter);
            if (listItem == item->second.end())
            {
              item->second.push_back(*listIter);
            }
            listIter++;
          }
          continue;
        }
      }
      continue;
    }
    std::list< std::string > list;
    if ((insertTranslation(list, line) == true) && (!list.empty()))
    {
      dictionary_.emplace(word, list);
    }
  }
  return true;
}

bool roletskaya::Dictionary::insert(const std::string& string)
{
  std::string line = string;
  std::string word = getElem(line);
  if (checkWord(word) == false)
  {
    throw std::invalid_argument("Incorrect input.\n");
    return false;
  }
  if (line.empty())
  {
    throw std::invalid_argument("Incorrect input.\n");
    return false;
  }
  auto item = dictionary_.find(word);
  if (item != dictionary_.end())
  {
    std::list< std::string > list;
    if (insertTranslation(list, line) == true)
    {
      if (item->second == list)
      {
        throw std::invalid_argument("This word is already in the dictionary.\n");
        return false;
      }
      else
      {
        auto listIter = list.begin();
        while (listIter != list.end())
        {
          auto listItem = std::find(item->second.begin(), item->second.end(), *listIter);
          if (listItem == item->second.end())
          {
            item->second.push_back(*listIter);
          }
          listIter++;
        }
        return true;
      }
    }
    throw std::invalid_argument("Incorrect input.\n");
    return false;
  }
  std::list< std::string > list;
  if ((insertTranslation(list, line) == true) && (!list.empty()))
  {
    dictionary_.emplace(word, list);
    return true;
  }
  return false;
}

bool roletskaya::Dictionary::findWordByTranslation(const std::string& trnsl, std::ostream& out)
{
  if (checkTranslation(trnsl) == false)
  {
    throw std::invalid_argument("Incorrect input.\n");
    return false;
  }
  for (auto iter = dictionary_.begin(); iter != dictionary_.end(); iter++)
  {
    for (auto iterList = iter->second.begin(); iterList != iter->second.end(); iterList++)
    {
      if (*iterList == trnsl)
      {
        out << "Translation is found: " << iter->first << " - " << trnsl << '\n';
        return true;
      }
    }
  }
  return false;
}

bool roletskaya::Dictionary::findTranslation(const std::string& word, std::ostream& out)
{
  if (checkWord(word) == false)
  {
    throw std::invalid_argument("Incorrect input.\n");
    return false;
  }
  auto item = dictionary_.find(word);
  if (item != dictionary_.end())
  {
    out << "Word is found: " << item->first << " - ";
    for (auto iter = item->second.begin(); iter != item->second.end(); iter++)
    {
      out << *iter << " ";
    }
    out << '\n';
  }
  else
  {
    return false;
  }
  return true;
}

bool roletskaya::Dictionary::deleteKey(const std::string& word)
{
  if (checkWord(word) == false)
  {
    throw std::invalid_argument("Incorrect input.\n");
    return false;
  }
  auto res = dictionary_.erase(word);
  if (res == false)
  {
    throw std::invalid_argument("No such word in the dictionary.\n");
    return false;
  }
  return true;
}

void roletskaya::Dictionary::print(std::ostream& out) const
{
  if (!out.good())
  {
    throw std::invalid_argument("Couldn't open the output file.\n");
    return;
  }
  if (dictionary_.empty())
  {
    out << "Dictionary is empty.\n";
    return;
  }
  out << "Dictionary:\n";
  for (auto iter = dictionary_.begin(); iter != dictionary_.end(); iter++)
  {
    out << iter->first << " - ";
    for (auto iterList = iter->second.begin(); iterList != iter->second.end(); iterList++)
    {
      out << *iterList << " ";
    }
    out << '\n';
  }
}

void roletskaya::Dictionary::translateText(const std::string& string, std::ostream& out)
{
  std::string line = string;
  std::string word = "";
  while (!line.empty())
  {
    word = getElem(line);
    if (checkWord(word) == false)
    {
      continue;
    }
    auto item = dictionary_.find(word);
    if (item != dictionary_.end())
    {
      out << item->first << " - ";
      for (auto iterList = item->second.begin(); iterList != item->second.end(); iterList++)
      {
        out << *iterList << " ";
      }
      out << '\n';
    }
    else
    {
      out << word << " : there's no such word in the dictionary.\n";
      continue;
    }
  }
}
