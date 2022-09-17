#include "Commands.hpp"

#include <ostream>
#include <fstream>
#include <algorithm>

#include "Messages.hpp"
#include "BasedParseFunction.hpp"

void yermakov::doGet(std::ostream &out, yermakov::TextDict &dict, std::string &description)
{
  std::string filename = getWord(description);
  std::string textname = getWord(description);
  if (!description.empty() || textname.empty())
  {
    throw std::exception();
  }
  std::ifstream input(filename);
  if (!input.is_open())
  {
    throw std::exception();
  }
  Text inputText;
  input >> inputText;
  dict.insert({textname, inputText});
}

void yermakov::doWrite(std::ostream &out, yermakov::TextDict &dict, std::string &description)
{
  std::string filename = getWord(description);
  std::string textname = getWord(description);
  if (!description.empty() || textname.empty())
  {
    throw std::exception();
  }
  std::fstream input(filename);
  if (!input.is_open())
  {
    throw std::exception();
  }
  auto toPrint = dict.find(textname);
  if (toPrint == dict.end())
  {
    throw std::exception();
  }
  input << (*toPrint).second;
}

void yermakov::doCalculateSize(std::ostream &out, yermakov::TextDict &dict, std::string &description)
{
  std::string textname = getWord(description);
  if (!description.empty())
  {
    throw std::exception();
  }
  auto toPrint = dict.find(textname);
  if (toPrint == dict.end())
  {
    throw std::exception();
  }
  out << (*toPrint).second.data_.text_.size();
}

void yermakov::doPrint(std::ostream &out, yermakov::TextDict &dict, std::string &description)
{
  std::string textname = getWord(description);
  if (!description.empty())
  {
    throw std::exception();
  }
  auto toPrint = dict.find(textname);
  if (toPrint == dict.end())
  {
    throw std::exception();
  }
  out << (*toPrint).second;
}

void yermakov::doCompress(std::ostream &out, yermakov::TextDict &dict, std::string &description)
{
  std::string oldName = getWord(description);
  std::string newName = getWord(description);
  if (newName.empty() || oldName.empty() || dict.find(oldName) == dict.end() || dict.at(oldName).isCompress_)
  {
    throw std::exception();
  }
  dict.insert({newName, compress(dict.at(oldName))});
}

void yermakov::doDecompress(std::ostream &out, yermakov::TextDict &dict, std::string &description)
{
  std::string oldName = getWord(description);
  std::string newName = getWord(description);
  if (newName.empty() || oldName.empty() || dict.find(oldName) == dict.end() || !dict.at(oldName).isCompress_)
  {
    throw std::exception();
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

void yermakov::doEfficiency(std::ostream &out, yermakov::TextDict &dict, std::string &description)
{
  std::string firstTextName = getWord(description);
  std::string secondTextName = getWord(description);
  auto firstText = dict.find(firstTextName);
  auto secondText = dict.find(secondTextName);
  bool isGoodName = firstText == dict.end() || secondText == dict.end();
  bool isRightCompress = (*firstText).second.isCompress_ || !(*secondText).second.isCompress_;
  bool isGoodLanguage = (*firstText).second.data_.language_ != (*secondText).second.data_.language_;
  if (isGoodName || isGoodLanguage || isRightCompress)
  {
    throw std::exception();
  }
  std::string str = (*secondText).second.data_.text_;
  std::size_t countStr = (std::count_if(str.begin(), str.end(), countSymb()) / 8);
  double eff = static_cast<double>((*firstText).second.data_.text_.size()) / countStr;
  out << "BEFORE COMPRESS: " << (*firstText).second.data_.text_.size() << "\n";
  out << "AFTER COMPRESS: " << countStr << "\n";
  out << "EFFICIENCY: " << eff << "\n";
}

void yermakov::doConcat(std::ostream &out, yermakov::TextDict &dict, std::string &description)
{
  std::string newName = getWord(description);
  std::string firstTextName = getWord(description);
  std::string secondTextName = getWord(description);
  if (newName.empty() || firstTextName.empty() || secondTextName.empty() || firstTextName == secondTextName)
  {
    throw std::exception();
  }
  auto firstText = dict.find(firstTextName);
  auto secondText = dict.find(secondTextName);
  bool isGoodName = firstText == dict.end() || secondText == dict.end();
  bool isRightCompress = (*firstText).second.isCompress_ || (*secondText).second.isCompress_;
  bool isGoodLanguage = (*firstText).second.data_.language_ != (*secondText).second.data_.language_;
  if (isGoodName || isGoodLanguage || isRightCompress)
  {
    throw std::exception();
  }
  Text newText;
  newText.data_.text_ = (*firstText).second.data_.text_ + (*secondText).second.data_.text_;
  newText.data_.calculateFrequency();
  dict.erase(firstText);
  dict.erase(secondText);
  dict.insert({newName, newText});
}

void yermakov::doCut(std::ostream &out, yermakov::TextDict &dict, std::string &description)
{
  try
  {
    std::string textname = getWord(description);
    unsigned long first = std::stoul(getWord(description));
    unsigned long last = std::stoul(getWord(description));
    auto text = dict.find(textname);
    if (text == dict.end())
    {
      throw std::exception();
    }
    (*dict.find(textname)).second.data_.text_ = (*dict.find(textname)).second.data_.text_.substr(first, last);
    (*dict.find(textname)).second.data_.calculateFrequency();
  }
  catch (...)
  {
    printInvalid(out) << "\n";
    return;
  }
}
