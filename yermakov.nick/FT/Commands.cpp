#include "Commands.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "Messages.hpp"
#include "BasedParseFunction.hpp"

void yermakov::doGet(yermakov::TextDict& dict, std::string& description)
{
  std::string filename = getAndEraseWord(description);
  std::string textname = getAndEraseWord(description);
  if (!description.empty() || textname.empty())
  {
    throw std::invalid_argument("wrong args");
  }
  std::ifstream input(filename);
  if (!input.is_open())
  {
    throw std::invalid_argument("wrong args");
  }
  Text inputText;
  input >> inputText;
  dict.insert({textname, inputText});
}

void yermakov::doWrite(const yermakov::TextDict& dict, std::string& description)
{
  std::string filename = getAndEraseWord(description);
  std::string textname = getAndEraseWord(description);
  if (!description.empty() || textname.empty())
  {
    throw std::invalid_argument("wrong args");
  }
  std::fstream input(filename);
  if (!input.is_open())
  {
    throw std::invalid_argument("wrong args");
  }
  auto toPrint = dict.find(textname);
  if (toPrint == dict.end())
  {
    throw std::invalid_argument("wrong args");
  }
  input << (*toPrint).second;
}

void yermakov::doCalculateSize(const yermakov::TextDict& dict, std::string& description)
{
  std::string textname = getAndEraseWord(description);
  if (!description.empty())
  {
    throw std::invalid_argument("wrong args");
  }
  auto toPrint = dict.find(textname);
  if (toPrint == dict.end())
  {
    throw std::invalid_argument("wrong args");
  }
  std::cout << (*toPrint).second.data_.text_.size();
}

void yermakov::doPrint(const yermakov::TextDict& dict, std::string& description)
{
  std::string textname = getAndEraseWord(description);
  if (!description.empty())
  {
    throw std::invalid_argument("wrong args");
  }
  auto toPrint = dict.find(textname);
  if (toPrint == dict.end())
  {
    throw std::invalid_argument("wrong args");
  }
  std::cout << (*toPrint).second;
}

void yermakov::doCompress(yermakov::TextDict& dict, std::string& description)
{
  std::string oldName = getAndEraseWord(description);
  std::string newName = getAndEraseWord(description);
  if (newName.empty() || oldName.empty() || dict.find(oldName) == dict.end() || dict.at(oldName).isCompress_)
  {
    throw std::invalid_argument("wrong args");
  }
  dict.insert({newName, compress(dict.at(oldName))});
}

void yermakov::doDecompress(yermakov::TextDict& dict, std::string& description)
{
  std::string oldName = getAndEraseWord(description);
  std::string newName = getAndEraseWord(description);
  if (newName.empty() || oldName.empty() || dict.find(oldName) == dict.end() || !dict.at(oldName).isCompress_)
  {
    throw std::invalid_argument("wrong args");
  }
  dict.insert({newName, decompress(dict.at(oldName))});
}

namespace
{
  struct countSymb
  {
    bool operator()(char ch)
    {
      return ch != ' ';
    }
  };
}

void yermakov::doEfficiency(const yermakov::TextDict& dict, std::string& description)
{
  std::string firstTextName = getAndEraseWord(description);
  std::string secondTextName = getAndEraseWord(description);
  auto firstText = dict.find(firstTextName);
  auto secondText = dict.find(secondTextName);
  bool isGoodName = firstText == dict.end() || secondText == dict.end();
  bool isRightCompress = (*firstText).second.isCompress_ || !(*secondText).second.isCompress_;
  bool isGoodLanguage = (*firstText).second.data_.language_ != (*secondText).second.data_.language_;
  if (isGoodName || isGoodLanguage || isRightCompress)
  {
    throw std::invalid_argument("wrong args");
  }
  std::string str = (*secondText).second.data_.text_;
  std::size_t countStr = (std::count_if(str.begin(), str.end(), countSymb()) / 8);
  double eff = static_cast< double >((*firstText).second.data_.text_.size()) / countStr;
  std::cout << "BEFORE COMPRESS: " << (*firstText).second.data_.text_.size() << "\n";
  std::cout << "AFTER COMPRESS: " << countStr << "\n";
  std::cout << "EFFICIENCY: " << eff << "\n";
}

void yermakov::doConcat(yermakov::TextDict& dict, std::string& description)
{
  std::string newName = getAndEraseWord(description);
  std::string firstTextName = getAndEraseWord(description);
  std::string secondTextName = getAndEraseWord(description);
  if (newName.empty() || firstTextName.empty() || secondTextName.empty() || firstTextName == secondTextName)
  {
    throw std::invalid_argument("wrong args");
  }
  auto firstText = dict.find(firstTextName);
  auto secondText = dict.find(secondTextName);
  bool isGoodName = firstText == dict.end() || secondText == dict.end();
  bool isRightCompress = (*firstText).second.isCompress_ || (*secondText).second.isCompress_;
  bool isGoodLanguage = (*firstText).second.data_.language_ != (*secondText).second.data_.language_;
  if (isGoodName || isGoodLanguage || isRightCompress)
  {
    throw std::invalid_argument("wrong args");
  }
  Text newText;
  newText.data_.text_ = (*firstText).second.data_.text_ + (*secondText).second.data_.text_;
  newText.data_.calculateFrequency();
  dict.erase(firstText);
  dict.erase(secondText);
  dict.insert({newName, newText});
}

void yermakov::doCut(yermakov::TextDict& dict, std::string& description)
{
  std::string textname = getAndEraseWord(description);
  unsigned long first = std::stoul(getAndEraseWord(description));
  unsigned long last = std::stoul(getAndEraseWord(description));
  auto text = dict.find(textname);
  if (text == dict.end())
  {
    throw std::invalid_argument("wrong args");
  }
  (*dict.find(textname)).second.data_.text_ = (*dict.find(textname)).second.data_.text_.substr(first, last);
  (*dict.find(textname)).second.data_.calculateFrequency();
}
