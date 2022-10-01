#include "CommandSystem.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <iterator>
#include <algorithm>
#include "iofmtguard.hpp"
#include "DelimiterIO.hpp"
#include "shannon-fano.hpp"
#include "io_struct.hpp"

gusarov::Command::Command():
  commandList_({
    {"add", std::bind(&gusarov::Command::add, this, std::ref(std::cin))},
    {"code", std::bind(&gusarov::Command::code, this, std::ref(std::cin))},
    {"encode", std::bind(&gusarov::Command::encode, this, std::ref(std::cin))},
    {"decode", std::bind(&gusarov::Command::decode, this, std::ref(std::cin))},
    {"merge", std::bind(&gusarov::Command::merge, this, std::ref(std::cin))},
    {"frequency", std::bind(&gusarov::Command::frequency, this, std::ref(std::cin), std::ref(std::cout))},
    {"print", std::bind(&gusarov::Command::print, this, std::ref(std::cin), std::ref(std::cout))},
    {"compare", std::bind(&gusarov::Command::compare, this, std::ref(std::cin), std::ref(std::cout))}
    })
{}

void gusarov::Command::add(std::istream& in)
{
  std::string fileName = "";
  std::string textKey = "";
  {
    iofmtguard guard(in);
    in >> std::noskipws;
    in >> DelimiterIO{' '};
    in >> fileIO{fileName};
    in >> DelimiterIO{' '};
    in >> textKey;
    in >> DelimiterIO{'\n'};
  }
  if (!in) {
    return;
  }
  std::ifstream file = openFile(fileName);
  texts_[textKey] = std::string(std::istreambuf_iterator< char >(file), std::istreambuf_iterator< char >());
  dictionaries_.erase(textKey);
  file.close();
}

void gusarov::Command::code(std::istream& in)
{
  std::string textKey = "";
  std::string codeKey = "";
  {
    iofmtguard guard(in);
    in >> std::noskipws;
    in >> DelimiterIO{' '};
    in >> textKey;
    in >> DelimiterIO{' '};
    in >> codeKey;
    in >> DelimiterIO{'\n'};
  }
  if (!in) {
    return;
  }
  dictionaries_[codeKey] = getCode(texts_.at(textKey));
  texts_.erase(codeKey);
}

void gusarov::Command::encode(std::istream& in)
{
  std::string textKey1;
  std::string codeKey;
  std::string textKey2;
  {
    iofmtguard guard(in);
    in >> std::noskipws;
    in >> DelimiterIO{' '};
    in >> textKey1;
    in >> DelimiterIO{' '};
    in >> codeKey;
    in >> DelimiterIO{' '};
    in >> textKey2;
    in >> DelimiterIO{'\n'};
  }
  if (!in) {
    return;
  }
  texts_[textKey2] = getEncodedText(texts_.at(textKey1), dictionaries_.at(codeKey));
  dictionaries_.erase(textKey2);
}

void gusarov::Command::decode(std::istream& in)
{
  std::string textKey1 = "";
  std::string codeKey = "";
  std::string textKey2 = "";
  {
    iofmtguard guard(in);
    in >> std::noskipws;
    in >> DelimiterIO{' '};
    in >> textKey1;
    in >> DelimiterIO{' '};
    in >> codeKey;
    in >> DelimiterIO{' '};
    in >> textKey2;
    in >> DelimiterIO{'\n'};
  }
  if (!in) {
    return;
  }
  texts_[textKey2] = getDecodedText(texts_.at(textKey1), dictionaries_.at(codeKey));
  dictionaries_.erase(textKey2);
}

void gusarov::Command::frequency(std::istream& in, std::ostream& out)
{
  std::string textKey = "";
  {
    iofmtguard gusard(in);
    in >> std::noskipws;
    in >> DelimiterIO{' '};
    in >> textKey;
    in >> DelimiterIO{'\n'};
  }
  if (!in) {
    return;
  }
  fanoAlphabet_t fanoAlphabet = getFrequency(texts_.at(textKey));
  for (auto iter : fanoAlphabet) {
    out << getSymbol(iter.symbol) << " " << iter.freq << "\n";
  }
}

void gusarov::Command::print(std::istream& in, std::ostream& out)
{
  std::string key = "";
  {
    iofmtguard gusard(in);
    in >> std::noskipws;
    in >> DelimiterIO{' '};
    in >> key;
    in >> DelimiterIO{'\n'};
  }
  if (!in) {
    return;
  }
  auto iterText = texts_.find(key);
  if (iterText != texts_.end()) {
    out << iterText->second << "\n";
    return;
  }

  for (auto iter : dictionaries_.at(key)) {
    out << getSymbol(iter.first) << " " << iter.second << "\n";
  }
}

void gusarov::Command::compare(std::istream& in, std::ostream& out)
{
  std::string textKey1 = "";
  std::string textKey2 = "";
  {
    iofmtguard gusard(in);
    in >> std::noskipws;
    in >> DelimiterIO{' '};
    in >> textKey1;
    in >> DelimiterIO{' '};
    in >> textKey2;
    in >> DelimiterIO{'\n'};
  }
  if (!in) {
    return;
  }
  size_t sizeFirst = getSize(texts_.at(textKey1));
  size_t sizeSecond = getSize(texts_.at(textKey2));
  out << "The size of the first file " << sizeFirst << " bytes\n";
  out << "The size of the second file " << sizeSecond << " bytes\n";
  out << "size difference: " << sizeFirst - sizeSecond << " bytes\n";
}

void gusarov::Command::merge(std::istream& in)
{
  std::string textKey1 = "";
  std::string codeKey1 = "";
  std::string textKey2 = "";
  std::string codeKey2 = "";
  std::string textKey3 = "";
  std::string codeKey3 = "";

  {
    iofmtguard gusard(in);
    in >> std::noskipws;
    in >> DelimiterIO{' '};
    in >> textKey1;
    in >> DelimiterIO{' '};
    in >> codeKey1;
    in >> DelimiterIO{' '};
    in >> textKey2;
    in >> DelimiterIO{' '};
    in >> codeKey2;
    in >> DelimiterIO{' '};
    in >> textKey3;
    if (in.peek() == ' ') {
      in >> DelimiterIO{' '};
      in >> codeKey3;
    }
    in >> DelimiterIO{'\n'};
  }
  if (!in) {
    return;
  }
  std::string newText = getDecodedText(texts_.at(textKey1), dictionaries_.at(codeKey1));
  newText += getDecodedText(texts_.at(textKey2), dictionaries_.at(codeKey2));

  texts_[textKey3] = newText;
  dictionaries_.erase(textKey3);

  if (!codeKey3.empty()) {

    dictionaries_[codeKey3] = getCode(texts_.at(textKey3));
    texts_[textKey3] = getEncodedText(texts_.at(textKey3), dictionaries_.at(codeKey3));

    texts_.erase(codeKey3);
    dictionaries_.erase(textKey3);
  }
}

void gusarov::Command::doCommand(const std::string& command)
{
  commandList_.at(command)();
  if (std::cin.rdstate() == std::ios::failbit) {
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    std::cin.clear();
    throw std::invalid_argument("incorrect input");
  }
}

