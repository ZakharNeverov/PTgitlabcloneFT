#include "CharDataStruct.hpp"

#include <fstream>
#include <algorithm>

yermakov::CharData::CharData():
  language_(),
  text_(),
  freqDict_()
{
}

namespace yermakov
{
  namespace
  {
    struct DictCreator
    {
      DictCreator(const FreqMap &map):
        frequencyDict_(map)
      {
      }

      void operator()(const char ch)
      {
        frequencyDict_[ch]++;
      }

      FreqMap frequencyDict_;
    };
  }
}

void yermakov::CharData::calculateFrequency()
{
  FreqMap frequencyDict;
  DictCreator creator(frequencyDict);
  std::for_each(text_.begin(), text_.end(), creator);
  freqDict_ = creator.frequencyDict_;
}

std::istream &yermakov::operator>>(std::istream &input, CharData &text)
{
  std::string helpString;
  input >> helpString;
  if (helpString != "LANGUAGE:")
  {
    input.setstate(std::ios::failbit);
  }
  input >> text.language_;
  input >> helpString;
  if (helpString != "TEXT:")
  {
    input.setstate(std::ios::failbit);
  }
  char c;
  input >> std::noskipws;
  input >> c;
  FreqMap frequencyDict;
  std::string textString;
  while (input >> c)
  {
    textString += c;
    frequencyDict[c]++;
  }
  text.freqDict_ = frequencyDict;
  text.text_ = textString;
  return input;
}
