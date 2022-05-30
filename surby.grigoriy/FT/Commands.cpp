#include "Commands.hpp"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "Dictionary.hpp"

void surby::addTextCommand(textes_map& texts, std::string& line)
{
  std::string name = surby::getNextArg(line, 1);
  std::string namefile = surby::getNextArg(line, 1);
  std::string text;

  if (namefile[0] == '\0')
  {
    std::getline(std::cin, text);
  }
  else
  {
    std::ifstream in(namefile);
    std::getline(in, text, '\0');
  }
  texts[name] = text;
}

void surby::getDictionaryCommand(textes_map& texts, std::string& line, std::ostream& out)
{
  std::string name = surby::getNextArg(line, 1);
  std::string namefile = surby::getNextArg(line, 1);

  if (texts.find(name) == texts.end()) {
    out << "text with name - " << name << " - exist\n";
  }
  else
  {
    if (namefile[0] == '\0')
    {
      std::string answer = surby::getBestDict(texts[name]);
      out << answer;
    }
    else
    {
      std::ofstream outFile(namefile);
      outFile << surby::getBestDict(texts[name]);
    }
  }
}

void surby::getTextCommand(textes_map& texts, std::string& line, std::ostream& out)
{
  std::string name = surby::getNextArg(line, 1);

  if (texts.find(name) == texts.end())
  {
    out << "text with name - " << name << " - exist\n";
    return;
  }
  std::string nanmefile = surby::getNextArg(line, 1);
  if (nanmefile[0] == '\0')
  {
    out << texts[name] << '\n';
  }
  else
  {
    std::ofstream outFile(nanmefile);
    outFile << texts[name];
  }
}

void surby::encodeCommand(textes_map& texts, std::string& line)
{
  std::string def = "default";
  std::string fileInText = surby::getNextArg(line, 1);
  std::string fileInDict = surby::getNextArg(line, 1);
  std::string name = surby::getNextArg(line, 1);
  std::ifstream inText(fileInText);

  if (fileInDict == def)
  {
    std::string text;
    std::getline(inText, text, '\0');
    texts[name] = surby::encode(text, def);
  }
  else
  {
    std::ifstream inDict(fileInDict);
    std::string text;
    std::getline(inText, text, '\0');
    std::string dict;
    std::getline(inDict, dict, '\0');
    texts[name] = surby::encode(text, dict);
  }
}

void surby::decodeCommand(textes_map& texts, std::string& line)
{
  std::string name1 = surby::getNextArg(line, 1);
  std::string fileInDict = surby::getNextArg(line, 1);
  std::ifstream inDict(fileInDict);
  std::string name2 = surby::getNextArg(line, 1);
  std::string dictionary;
  std::getline(inDict, dictionary, '\0');
  if (name2[0] == '\0')
  {
    texts[name1] = surby::decode(texts[name1], dictionary);
  }
  else
  {
    texts[name2] = surby::decode(texts[name1], dictionary);
  }
}

void surby::encodeKeyCommand(textes_map& texts, std::string& line)
{
  std::string def = "default";
  std::string nameIn = surby::getNextArg(line, 1);
  std::string fileInDict = surby::getNextArg(line, 1);
  std::string nameOut = surby::getNextArg(line, 1);
  std::string answer;
  if (fileInDict == def)
  {
    answer = surby::encode(texts[nameIn], def);
  }
  else
  {
    std::ifstream inDict(fileInDict);
    std::string dict;
    std::getline(inDict, dict);
    answer = surby::encode(texts[nameIn], dict);
  }
  if (nameOut[0] == '\0')
  {
    texts[nameIn] = answer;
  }
  else
  {
    texts[nameOut] = answer;
  }
}

void surby::checkDifferenceCommand(textes_map& texts, std::string& line, std::ostream& out)
{
  std::string def = "default";
  std::string name = surby::getNextArg(line, 1);
  std::string fileIn = surby::getNextArg(line, 1);
  int memoryBefore = 0;
  int memoryAfter = 0;
  std::string text = texts[name];
  memoryBefore = text.size() * sizeof(char);
  if (fileIn == def)
  {
    text = surby::encode(text, def);
  }
  else
  {
    std::ifstream in(fileIn);
    std::string dict;
    std::getline(in, dict, '\0');
    text = surby::encode(text, dict);
  }
  memoryAfter = text.size() / 8;
  out << "memoryBefore  = " << memoryBefore << " bytes\nmemoryAfter = " << memoryAfter << " bytes" << std::endl;
  int dif = memoryBefore - memoryAfter;
  out << "difference =  " << dif << " bytes" << std::endl;
  out << "compression = " << 100 - 100 * dif / memoryBefore << '%' << std::endl;
}
