#include <stdexcept>

#include "Dictionary.hpp"
#include "Utilites.hpp"

using map = std::map< std::string, std::list< std::string > >;

roletskaya::Dictionary::Dictionary() :
  dictionary_{}
{}

roletskaya::Dictionary::Dictionary(Dictionary&& src) noexcept :
  dictionary_{std::move(src.dictionary_)}
{}

roletskaya::Dictionary& roletskaya::Dictionary::operator=(Dictionary&& src) noexcept
{
  dictionary_ = std::move(src.dictionary_);
  return *this;
}

bool roletskaya::Dictionary::checkWord(const std::string& string) const
{
  for (int i = 0; i < string.length(); i++)
  {
    if (string[i] > 'z' || string[i] < 'a')
    {
      return false;
    }
  }
  return true;
}

bool roletskaya::Dictionary::checkTranslation(const std::string& string) const
{
  for (int i = 0; i < string.length(); i++)
  {
    if (string[i] > 'ÿ' || string[i] < 'à')
    {
      return false;
    }
  }
  return true;
}

void roletskaya::Dictionary::createDictionary(std::istream& in)
{
  if (!in.good())
  {
    throw std::invalid_argument("Couldn't open the input file.\n");
    return;
  }
  std::string line = "";
  std::string word = "";
  int count = 0;
  while (!in.eof())
  {
    count = 0;
    std::getline(in, line);
    word = getElem(line);
    if (checkWord(word) == false)
    {
      continue;
    }
    if (line == "")
    {
      continue;
    }
    auto item = dictionary_.find(word);
    if (item != dictionary_.end())
    {
      continue;
    }
    if (line.size() == 0.0)
    {
      continue;
    }
    std::string translation = "";
    std::list< std::string > list;
    while (line.size() != 0.0)
    {
      translation = getElem(line);
      if (checkTranslation(translation) == false)
      {
        continue;
      }
      list.push_back(translation);
      count++;
    }
    if (count > 0)
    {
      dictionary_.emplace(word, list);
    }
  }
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
  auto item = dictionary_.find(word);
  if (item != dictionary_.end())
  {
    throw std::invalid_argument("This word is already in the dictionary.\n");
    return false;
  }
  if (line.size() == 0.0)
  {
    throw std::invalid_argument("Incorrect input.\n");
    return false;
  }
  std::string translation = "";
  std::list< std::string > list;
  while (line.size() != 0.0)
  {
    translation = getElem(line);
    if (checkTranslation(translation) == false)
    {
      throw std::invalid_argument("Incorrect input.\n");
      return false;
    }
    list.push_back(translation);
  }
  dictionary_.emplace(word, list);
  return true;
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
  out << "No such word in the dictionary.\n";
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
  } else
  {
    out << "No such word in the dictionary.\n";
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
    throw std::invalid_argument("Couldn't open the output file.");
    return;
  }
  if (dictionary_.size() == 0.0)
  {
    out << "Dictionary is empty.\n";
    return;
  }
  out << "Dictionary:\n";
  for (auto iter = dictionary_.begin(); iter != dictionary_.end(); iter++)
  {
    out << iter->first << " - ";
    auto item = dictionary_.find(iter->first);
    for (auto iter = item->second.begin(); iter != item->second.end(); iter++)
    {
      out << *iter << " ";
    }
    out << '\n';
  }
}

void roletskaya::Dictionary::translateText(std::istream& in, std::ostream& out)
{
  if (!in.good())
  {
    throw std::invalid_argument("Couldn't open the input file.");
    return;
  }
  if (!out.good())
  {
    throw std::invalid_argument("Couldn't open the output file.");
    return;
  }
  std::string line = "";
  std::string word = "";
  while (!in.eof())
  {
    std::getline(in, line);
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
        for (auto iter = item->second.begin(); iter != item->second.end(); iter++)
        {
          out << *iter << " ";
        }
        out << '\n';
      } else
      {
        out << word << " : there's no such word in the dictionary.\n";
        continue;
      }
    }
  }
}

void roletskaya::Dictionary::inputResult(map dict)
{
  dictionary_ = std::move(dict);
}
