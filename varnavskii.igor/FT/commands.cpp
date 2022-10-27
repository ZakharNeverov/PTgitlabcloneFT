#include "commands.h"
#include <limits>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <iterator>
#include <algorithm>
#include "ioFmtGuard.hpp"
#include "ioDelimeter.hpp"
#include "shannonFano.h"
#include "ioStruct.h"

void varnavskii::add(texts_t& texts, dicts_t& dictionaries, std::istream& in)
{
  std::string fileName = "";
  std::string textKey = "";
  {
    IoFmtGuard guard(in);
    in >> std::noskipws;
    in >> DelimeterIO{' '};
    in >> fileIO{fileName};
    in >> DelimeterIO{' '};
    in >> textKey;
    in >> DelimeterIO{'\n'};
  }
  if (!in)
  {
    return;
  }
  std::ifstream file = openFile(fileName);
  texts[textKey] = std::string(std::istreambuf_iterator< char >(file), std::istreambuf_iterator< char >());
  dictionaries.erase(textKey);
  file.close();
}

void varnavskii::code(texts_t& texts, dicts_t& dictionaries, std::istream& in)
{
  std::string textKey = "";
  std::string codeKey = "";
  {
    IoFmtGuard guard(in);
    in >> std::noskipws;
    in >> DelimeterIO{' '};
    in >> textKey;
    in >> DelimeterIO{' '};
    in >> codeKey;
    in >> DelimeterIO{'\n'};
  }
  if (!in)
  {
    return;
  }
  dictionaries[codeKey] = getCode(texts.at(textKey));
  texts.erase(codeKey);
}

void varnavskii::encode(texts_t& texts, dicts_t& dictionaries, std::istream& in)
{
  std::string textKey1;
  std::string codeKey;
  std::string textKey2;
  {
    IoFmtGuard guard(in);
    in >> std::noskipws;
    in >> DelimeterIO{' '};
    in >> textKey1;
    in >> DelimeterIO{' '};
    in >> codeKey;
    in >> DelimeterIO{' '};
    in >> textKey2;
    in >> DelimeterIO{'\n'};
  }
  if (!in)
  {
    return;
  }
  texts[textKey2] = getEncodedText(texts.at(textKey1), dictionaries.at(codeKey));
  dictionaries.erase(textKey2);
}

void varnavskii::decode(texts_t& texts, dicts_t& dictionaries, std::istream& in)
{
  std::string textKey1 = "";
  std::string codeKey = "";
  std::string textKey2 = "";
  {
    IoFmtGuard guard(in);
    in >> std::noskipws;
    in >> DelimeterIO{' '};
    in >> textKey1;
    in >> DelimeterIO{' '};
    in >> codeKey;
    in >> DelimeterIO{' '};
    in >> textKey2;
    in >> DelimeterIO{'\n'};
  }
  if (!in)
  {
    return;
  }
  texts[textKey2] = getDecodedText(texts.at(textKey1), dictionaries.at(codeKey));
  dictionaries.erase(textKey2);
}

void varnavskii::frequency(texts_t& texts, std::istream& in, std::ostream& out)
{
  std::string textKey = "";
  {
    IoFmtGuard guard(in);
    in >> std::noskipws;
    in >> DelimeterIO{' '};
    in >> textKey;
    in >> DelimeterIO{'\n'};
  }
  if (!in)
  {
    return;
  }
  fanoAlphabet_t fanoAlphabet = getFrequency(texts.at(textKey));
  for (auto iter: fanoAlphabet)
  {
    out << getSymbol(iter.symbol) << " " << iter.freq << "\n";
  }
}

void varnavskii::print(texts_t& texts, dicts_t& dictionaries, std::istream& in, std::ostream& out)
{
  std::string key = "";
  {
    IoFmtGuard gusard(in);
    in >> std::noskipws;
    in >> DelimeterIO{' '};
    in >> key;
    in >> DelimeterIO{'\n'};
  }
  if (!in)
  {
    return;
  }
  auto iterText = texts.find(key);
  if (iterText != texts.end())
  {
    out << iterText->second << "\n";
    return;
  }

  for (auto iter: dictionaries.at(key))
  {
    out << getSymbol(iter.first) << " " << iter.second << "\n";
  }
}

void varnavskii::merge(texts_t& texts, dicts_t& dictionaries, std::istream& in)
{
  std::string textKey1 = "";
  std::string codeKey1 = "";
  std::string textKey2 = "";
  std::string codeKey2 = "";
  std::string textKey3 = "";
  std::string codeKey3 = "";

  {
    IoFmtGuard gusard(in);
    in >> std::noskipws;
    in >> DelimeterIO{' '};
    in >> textKey1;
    in >> DelimeterIO{' '};
    in >> codeKey1;
    in >> DelimeterIO{' '};
    in >> textKey2;
    in >> DelimeterIO{' '};
    in >> codeKey2;
    in >> DelimeterIO{' '};
    in >> textKey3;
    if (in.peek() == ' ')
    {
      in >> DelimeterIO{' '};
      in >> codeKey3;
    }
    in >> DelimeterIO{'\n'};
  }
  if (!in)
  {
    return;
  }
  std::string newText = getDecodedText(texts.at(textKey1), dictionaries.at(codeKey1));
  newText += getDecodedText(texts.at(textKey2), dictionaries.at(codeKey2));

  texts[textKey3] = newText;
  dictionaries.erase(textKey3);

  if (!codeKey3.empty())
  {
    dictionaries[codeKey3] = getCode(texts.at(textKey3));
    texts[textKey3] = getEncodedText(texts.at(textKey3), dictionaries.at(codeKey3));
    texts.erase(codeKey3);
    dictionaries.erase(textKey3);
  }
}
