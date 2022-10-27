#include "consoleCommands.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "iofunctions.hpp"
#include "comExceptions.hpp"

namespace
{
  struct NotCodePred
  {
    bool operator()(char c)
    {
      return c != '1' && c != '0';
    }
  };

  double getDifference(const std::string& charStr, const std::string& byteStr)
  {
    return 100.0 * byteStr.size() / (8.0 * charStr.size());
  }
  bool isCode(const std::string& text)
  {
    std::vector< char > notCode{};
    std::copy_if(text.begin(), text.end(), std::back_inserter(notCode), NotCodePred{});
    return notCode.size() == 0;
  }
}

void neverov::createText(std::string& cmd, texts_t& texts, std::istream& in)
{
  args_t args = getArgs(cmd, 1, 1);

  if (texts.find(args[0]) != texts.end())
  {
    throw textAlreadyExists;
  }
  std::string tempText = "";
  if (args.size() == 2)
  {
    std::ifstream fin = std::ifstream(args[1]);
    if (!fin.good())
    {
      throw noFile;
    }
    inputText(fin, tempText);
  }
  else
  {
    inputText(in, tempText);
  }
  texts.insert({args[0], tempText});
}

void neverov::encode(std::string& cmd, texts_t& texts)
{
  args_t args = getArgs(cmd, 2, 1);
  if (texts.find(args[0]) == texts.end())
  {
    throw noText;
  }
  if (texts.find(args[1]) != texts.end())
  {
    throw textAlreadyExists;
  }
  std::string encodingText = texts.at(args[0]);

  std::string totalText = "";
  if (args.size() == 3)
  {
    std::ifstream in(args[2]);
    if (!in.good())
    {
      throw noFile;
    }

    HTree cur = HTree(in);
    texts.insert({args[1], cur.encode(texts.at(args[0]))});
  }
  else
  {
    HTree cur = HTree(texts.at(args[0]));
    texts.insert({args[1], cur.encode(texts.at(args[0]))});

    std::ofstream fout(args[0] + "Dict.txt");
    outputHuffmanCode(fout, cur);
  }
}

void neverov::decode(std::string& cmd, texts_t& texts)
{
  args_t args = getArgs(cmd, 3, 0);
  if (texts.find(args[0]) == texts.end())
  {
    throw noText;
  }
  if (texts.find(args[1]) != texts.end())
  {
    throw textAlreadyExists;
  }
  std::ifstream fin(args[2]);
  HTree temp(fin);
  std::string decoded = temp.decode(texts.at(args[0]));
  texts.insert({args[1], decoded});
}

void neverov::getText(std::string& cmd, const texts_t& texts, std::ostream& out)
{
  args_t args = getArgs(cmd, 1, 1);
  std::string totalText = "";
  if (texts.find(args[0]) == texts.end())
  {
    throw noText;
  }

  totalText = texts.at(args[0]);
  if (args.size() == 2)
  {
    std::ofstream out(args[1]);
    if (!out.good())
    {
      throw notOpenedFile;
    }
    out << totalText;
  }
  else
  {
    out << totalText << '\n';
  }
}

void neverov::mergeText(std::string& cmd, texts_t& texts)
{
  args_t args = getArgs(cmd, 3, 0);
  if (texts.find(args[0]) == texts.end() || texts.find(args[1]) == texts.end())
  {
    throw noText;
  }
  if (texts.find(args[2]) != texts.end())
  {
    throw textAlreadyExists;
  }
  bool fIsCode = isCode(texts.at(args[0]));
  bool sIsCode = isCode(texts.at(args[1]));
  if (fIsCode == sIsCode)
  {
    texts.insert({args[2], texts.at(args[0]) + texts.at(args[1])});
  }
  else
  {
    throw mergeTextNCode;
  }
}

void neverov::getCompDiff(std::string& cmd, const texts_t& texts, std::ostream& out)
{
  args_t args = getArgs(cmd, 1, 1);
  if (texts.find(args[0]) == texts.end())
  {
    throw noText;
  }
  std::string encodingText = texts.at(args[0]);
  double difference = 0.0;
  if (args.size() == 2)
  {
    std::ifstream in(args[1]);
    if (!in.good())
    {
      throw noFile;
    }
    HTree cur = HTree(in);
    std::string encodedText = cur.encode(texts.at(args[0]));
    difference = ::getDifference(encodingText, encodedText);
  }
  else
  {
    HTree cur = HTree(texts.at(args[0]));
    std::ofstream fout(args[0] + "Dict.txt");
    outputHuffmanCode(fout, cur);
    std::string encodedText = cur.encode(texts.at(args[0]));
    difference = ::getDifference(encodingText, encodedText);
  }
  std::streamsize prevPrec = out.precision();
  out << std::setprecision(2) << difference << "%\n";
  out << std::setprecision(prevPrec);
}

void neverov::getDict(std::string& cmd, const texts_t& texts)
{
  args_t args = getArgs(cmd, 1, 0);
  if (texts.find(args[0]) == texts.end())
  {
    throw noText;
  }
  HTree temp(texts.at(args[0]));
  outputHuffmanCode(std::cout, temp);
}
