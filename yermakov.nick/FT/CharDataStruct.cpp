#include "CharDataStruct.hpp"

#include <fstream>
#include <sstream>

yermakov::CharData::CharData():
  language_(),
  text_(),
  freqDict_()
{}

void yermakov::CharData::calculateFrequency()
{
  std::stringstream stream(text_);
  FreqMap frequencyDict;
  char c;
  stream >> std::noskipws;
  std::string textString;
  while (stream >> c)
  {
    textString += c;
    frequencyDict[c]++;
  }
  freqDict_ = frequencyDict;
}

std::istream& yermakov::operator>>(std::istream& input, CharData& text)
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
