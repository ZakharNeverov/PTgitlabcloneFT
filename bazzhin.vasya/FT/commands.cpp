#include "commands.hpp"

#include <iostream>

#include "messages.hpp"

bazzhin::Commands::Commands(std::istream& in, std::ostream& out, bazzhin::dictionaries& dicts):
  in_(in),
  out_(out),
  dicts_(dicts)
{
}

bazzhin::dictionary bazzhin::Commands::createCommonDictionary()
{
  dictionary tmp;
  for (auto& dict : dicts_)
  {
    for (auto& i : dict.second)
    {
      auto find = tmp.find(i.first);
      if (tmp.empty() || find == tmp.end())
      {
        tmp.emplace(i.first, i.second);
      }
      else
      {
        find->second += i.second;
      }
    }
  }
  return tmp;
}

void bazzhin::Commands::createDictionary()
{
  std::string name = "";
  in_ >> name;
  if (!in_ && !in_.eof())
  {
    bazzhin::printInvalidArguments(out_);
    in_.clear();
    return;
  }
  auto it = dicts_.find(name);
  if (it != dicts_.end())
  {
    bazzhin::printAlreadyExists(out_);
  }
  else
  {
    dictionary dict;
    dicts_.insert(std::make_pair(name, dict));
  }
}

void bazzhin::Commands::deleteDictionary()
{
  std::string name = "";
  in_ >> name;
  if (!in_ && !in_.eof())
  {
    bazzhin::printInvalidArguments(out_);
    in_.clear();
    return;
  }
  auto it = dicts_.find(name);
  if (it == dicts_.end())
  {
    bazzhin::printNotExists(out_) << "\n";
  }
  else
  {
    dicts_.erase(it);
  }
}

void bazzhin::Commands::printDictionary()
{
  std::string name = "";
  in_ >> name;
  if (!in_ && !in_.eof())
  {
    bazzhin::printInvalidArguments(out_) << "\n";
    in_.clear();
    return;
  }
  auto it = dicts_.find(name);
  if (it == dicts_.end())
  {
    bazzhin::printNotExists(out_) << "\n";
  }
  else
  {
    auto& words = it->second;
    if (words.empty())
    {
      bazzhin::printEmpty(out_) << "\n";
      return;
    }
    for (auto& item : words)
    {
      out_ << item.first << " " << item.second << "\n";
    }
  }
}

void bazzhin::Commands::searchWordInDictionary()
{
  std::string name = "";
  in_ >> name;
  if (!in_ && !in_.eof())
  {
    bazzhin::printInvalidArguments(out_) << "\n";
    in_.clear();
    return;
  }

  if (in_.peek() == '\n')
  {
    bazzhin::printNotEnoughArguments(out_) << "\n";
    return;
  }

  std::string word;
  in_ >> word;

  if (!in_ && !in_.eof())
  {
    bazzhin::printInvalidArguments(out_) << "\n";
    in_.clear();
    return;
  }

  auto it = dicts_.find(name);
  if (it == dicts_.end())
  {
    bazzhin::printNotExists(out_) << "\n";
  }
  else
  {
    auto& words = it->second;
    if (words.empty())
    {
      bazzhin::printEmpty(out_) << "\n";
      return;
    }
    auto needWord = words.find(word);
    if (needWord == words.end())
    {
      bazzhin::printMissingWord(out_) << "\n";
    }
    else
    {
      out_ << needWord->first << " " << needWord->second << "\n";
    }
  }
}

void bazzhin::Commands::deleteWordInDictionary()
{
  std::string name = "";
  in_ >> name;
  if (!in_ && !in_.eof())
  {
    bazzhin::printInvalidArguments(out_) << "\n";
    in_.clear();
    return;
  }

  if (in_.peek() == '\n')
  {
    bazzhin::printNotEnoughArguments(out_) << "\n";
    return;
  }

  std::string word;
  in_ >> word;

  if (!in_ && !in_.eof())
  {
    bazzhin::printInvalidArguments(out_) << "\n";
    in_.clear();
    return;
  }

  auto it = dicts_.find(name);
  if (it == dicts_.end())
  {
    bazzhin::printNotExists(out_) << "\n";
  }
  else
  {
    auto& words = it->second;
    if (words.empty())
    {
      bazzhin::printEmpty(out_) << "\n";
      return;
    }
    auto needWord = words.find(word);
    if (needWord != words.end())
    {
      words.erase(needWord);
    }
  }
}

void bazzhin::Commands::deleteWordInAllDictionaries()
{
  std::string word = "";
  in_ >> word;
  if (!in_ && !in_.eof())
  {
    bazzhin::printInvalidArguments(out_) << "\n";
    in_.clear();
    return;
  }
  for (auto& dict : dicts_)
  {
    dict.second.erase(word);
  }
}

void bazzhin::Commands::printAllWords()
{
  dictionary tmp = createCommonDictionary();
  if (tmp.empty())
  {
    bazzhin::printEmpty(out_) << "\n";
  }

  for (auto& it : tmp)
  {
    out_ << it.first << " " << it.second << "\n";
  }
}

void bazzhin::Commands::addWordsFromText()
{
  std::string name = "";
  in_ >> name;
  if (!in_ && !in_.eof())
  {
    bazzhin::printInvalidArguments(out_) << "\n";
    in_.clear();
    return;
  }
  auto it = dicts_.find(name);
  std::string word = "";
  if (it != dicts_.end())
  {
    while (in_.peek() != '\n')
    {
      in_ >> word;
      bazzhin::addWordAtDictionary(word, it->second);
    }
  }
  else
  {
    bazzhin::printNotExists(out_) << "\n";
  }

}

void bazzhin::Commands::printPeriodOfWord()
{
  std::string word;
  in_ >> word;

  if (!in_ && !in_.eof())
  {
    bazzhin::printInvalidArguments(out_) << "\n";
    in_.clear();
    return;
  }

  int counter = 0;
  for (auto& dict : dicts_)
  {
    auto it = dict.second.find(word);
    if (it != dict.second.end())
    {
      counter += it->second;
    }
  }

  out_ << "Period of word " << word << " is : " << counter << "\n";
}

void bazzhin::Commands::insertWord()
{
  std::string name = "";
  in_ >> name;
  if (!in_ && !in_.eof())
  {
    bazzhin::printInvalidArguments(out_) << "\n";
    in_.clear();
    return;
  }

  if (in_.peek() == '\n')
  {
    bazzhin::printNotEnoughArguments(out_) << "\n";
    return;
  }

  std::string word;
  in_ >> word;

  if (!in_ && !in_.eof())
  {
    bazzhin::printInvalidArguments(out_) << "\n";
    in_.clear();
    return;
  }

  if (in_.peek() == '\n')
  {
    bazzhin::printNotEnoughArguments(out_) << "\n";
    return;
  }

  int period;
  in_ >> period;

  if (!in_ && !in_.eof())
  {
    bazzhin::printInvalidArguments(out_) << "\n";
    in_.clear();
    return;
  }

  auto dict = dicts_.find(name);
  if (dict == dicts_.end())
  {
    bazzhin::printNotExists(out_) << "\n";
  }
  else
  {
    auto find = dict->second.find(word);
    if (find == dict->second.end())
    {
      dict->second.emplace(word, period);
    }
    else
    {
      find->second += period;
    }
  }
}

void bazzhin::Commands::printMainWords()
{
  dictionary tmp = createCommonDictionary();
  dictionary top;
  for (auto& i : tmp)
  {
    if (top.size() < 3)
    {
      top.emplace(i.first, i.second);
    }
    else
    {
      std::string minKey = top.begin()->first;
      int minValue = top.begin()->second;
      for (auto& el : top)
      {
        if (el.second < minValue)
        {
          minKey = el.first;
          minValue = el.second;
        }
      }
      if (minValue < i.second)
      {
        top.erase(minKey);
        top.emplace(i.first, i.second);
      }
    }
  }

  if (top.empty())
  {
    bazzhin::printEmpty(out_) << "\n";
  }
  else
  {
    for (auto& el : top)
    {
      out_ << el.first << " " << el.second << "\n";
    }
  }
}
