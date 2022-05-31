#include "dict-commands.hpp"
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <numeric>
#include <algorithm>
#include "stream-guard.hpp"
#include "delimeter.hpp"

using FreqDictionary = std::map< std::string, unsigned int >;
using DictionaryArray = std::unordered_map< std::string, FreqDictionary >;
using WordAndFreq = std::pair< std::string, unsigned int >;

const unsigned short int RECORD_PRECISION = 5;
const std::string allowedSymbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-\'";

namespace
{
  std::string& processString(std::string& str)
  {
    std::transform(str.begin(), str.end(), str.begin(), std::toupper);
    for (size_t i = 0; i < str.length(); ++i)
    {
      if (allowedSymbols.find(str[i]) == allowedSymbols.npos)
      {
        str.erase(i, 1);
        --i;
      }
    }
    return str;
  }
  bool compAscendingFreq(WordAndFreq element1, WordAndFreq element2)
  {
    return element1.second < element2.second;
  }
  bool compDescendingFreq(WordAndFreq element1, WordAndFreq element2)
  {
    return element1.second > element2.second;
  }
  unsigned int accumulateDictSum(unsigned int sum, WordAndFreq record)
  {
    return sum + record.second;
  }
  void insertWord(std::string word, FreqDictionary& dict)
  {
    processString(word);
    if (!word.empty())
    {
      if (dict.find(word) == dict.end())
      {
        dict.insert(std::make_pair(word, 1));
      }
      else
      {
        ++dict[word];
      }
    }
  }
  unsigned int evaluateWordNumber(FreqDictionary& dict)
  {
    return std::accumulate(dict.begin(), dict.end(), 0, accumulateDictSum);
  }
  size_t getWordLength(size_t sum, WordAndFreq pair1)
  {
    return sum + pair1.first.size();
  }
  size_t evaluateAverageWordLength(FreqDictionary& dict)
  {
    size_t sumWordLength = std::accumulate(dict.begin(), dict.end(), 0, getWordLength);
    unsigned int wordNumber = dict.size();
    return sumWordLength / wordNumber;
  }
}

void nefedev::outInvalidCommand(std::ostream& out)
{
  out << "INVALID COMMAND\n";
}

void nefedev::cleanIStream(std::istream& in)
{
  std::cin.clear();
  std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
}

void nefedev::insert(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  std::string word = "";
  in >> dictName;
  in >> word;
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  processString(word);
  if (!word.empty())
  {
    if (dictArray.find(dictName) == dictArray.end())
    {
      FreqDictionary dict;
      dictArray.insert(std::make_pair(dictName, dict));
    }
    if(dictArray[dictName].find(word) == dictArray[dictName].end())
    {
      dictArray[dictName].insert(std::make_pair(word, 1));
    }
    else
    {
      ++dictArray[dictName][word];
    }
    out << "Inserted word \"" << word << "\" to dictionary " << dictName << '\n';
  }
  else
  {
    throw std::invalid_argument("INVALID WORD");
  }
}

void nefedev::readText(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    FreqDictionary dict;
    dictArray.insert(std::make_pair(dictName, dict));
  }
  std::string input = "";
  std::getline(std::cin, input);
  input.erase(0, 1);
  std::string word;
  input += ' ';
  while (input.length() > 0)
  {
    word = input.substr(0, input.find(' '));
    input.erase(0, input.find(' ') + 1);
    insertWord(word, dictArray[dictName]);
  }
  if (dictArray[dictName].empty())
  {
    dictArray.erase(dictName);
    throw std::invalid_argument("Could not find any valid words");
  }
  else
  {
    out << "Inserted all words from the text\n";
  }
}

void nefedev::readFile(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    FreqDictionary dict;
    dictArray.insert(std::make_pair(dictName, dict));
  }
  std::string fileName;
  std::string input = "";
  in >> fileName;
  std::ifstream fin(fileName);
  if (fin.is_open())
  {
    while (!fin.eof())
    {
      fin >> input;
      insertWord(input, dictArray[dictName]);
    }
    if (dictArray[dictName].empty())
    {
      dictArray.erase(dictName);
      throw std::invalid_argument("Could not find any valid words");
    }
    else
    {
      out << "Inserted all words from file \"" << fileName << "\"\n";
    }
  }
  else
  {
    throw std::invalid_argument("There is no such file");
  }
}

void nefedev::deleteWord(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    throw std::invalid_argument("Dictionary does not exist");
  }
  else
  {
    std::string input = "";
    in >> input;
    std::transform(input.begin(), input.end(), input.begin(), std::toupper);
    if (dictArray[dictName].find(input) == dictArray[dictName].end())
    {
      throw std::invalid_argument("There is no such word in this dictionary");
    }
    else if (dictArray[dictName][input] > 1)
    {
      --dictArray[dictName][input];
      out << "Deleted 1 entrance of the word from dictionary\n";
    }
    else
    {
      dictArray[dictName].erase(input);
      out << "Deleted 1 entrance of the word from dictionary\n";
      if (dictArray[dictName].empty())
      {
        dictArray.erase(dictName);
        out << "After the operation dictionary became empty, so it was deleted\n";
      }
    }
  }
}

void nefedev::deleteAllWords(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    throw std::invalid_argument("Dictionary does not exist");
  }
  else
  {
    std::string input = "";
    in >> input;
    std::transform(input.begin(), input.end(), input.begin(), std::toupper);
    if (dictArray[dictName].find(input) == dictArray[dictName].end())
    {
      throw std::invalid_argument("There is no such word in this dictionary");
    }
    else
    {
      dictArray[dictName].erase(input);
      out << "Deleted all entrances of the word\n";
      if (dictArray[dictName].empty())
      {
        dictArray.erase(dictName);
      }
    }
  }
}

void nefedev::wordNumber(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    throw std::invalid_argument("Dictionary does not exist");
  }
  else
  {
    out << evaluateWordNumber(dictArray[dictName]) << '\n';
  }
}

void nefedev::printAlphabetic(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    throw std::invalid_argument("Dictionary does not exist");
  }
  else
  {
    std::string order = "";
    std::getline(in, order);
    order.erase(0, 1);
    if (order == "ASCENDING" || order == "")
    {
      unsigned int wordNumber = evaluateWordNumber(dictArray[dictName]);
      unsigned int wordlength = evaluateAverageWordLength(dictArray[dictName]);
      nefedev::StreamGuard guard(out);
      out << std::fixed << std::setprecision(5);
      for (FreqDictionary::iterator iter = dictArray[dictName].begin(); iter != dictArray[dictName].end(); ++iter)
      {
        out << std::setw(wordlength) << iter->first << " - " << static_cast< double >(iter->second) / wordNumber << '\n';
      }
    }
    else if (order == "DESCENDING")
    {
      unsigned int wordNumber = evaluateWordNumber(dictArray[dictName]);
      unsigned int wordlength = evaluateAverageWordLength(dictArray[dictName]);
      nefedev::StreamGuard guard(out);
      out << std::fixed << std::setprecision(5);
      for (FreqDictionary::reverse_iterator iter = dictArray[dictName].rbegin(); iter != dictArray[dictName].rend(); ++iter)
      {
        out << std::setw(wordlength) << iter->first << " - " << static_cast< double >(iter->second) / wordNumber << '\n';
      }
    }
    else
    {
      throw std::invalid_argument("INVALID ARGUMENT\n");
    }
  }
}

void nefedev::printFrequency(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    throw std::invalid_argument("Dictionary does not exist");
  }
  else
  {
    std::vector< WordAndFreq > vect(dictArray[dictName].begin(), dictArray[dictName].end());
    std::string order;
    std::getline(in, order);
    order.erase(0, 1);
    if (order == "ASCENDING" || order == "")
    {
      std::sort(vect.begin(), vect.end(), compAscendingFreq);
      unsigned int wordNumber = evaluateWordNumber(dictArray[dictName]);
      unsigned int wordlength = evaluateAverageWordLength(dictArray[dictName]);
      nefedev::StreamGuard guard(out);
      out << std::fixed << std::setprecision(5);
      for (std::vector< WordAndFreq >::iterator iter = vect.begin(); iter != vect.end(); ++iter)
      {
        out << std::setw(wordlength) << iter->first << " - " << static_cast< double >(iter->second) / wordNumber << '\n';
      }
    }
    else if (order == "DESCENDING")
    {
      std::sort(vect.begin(), vect.end(), compDescendingFreq);
      unsigned int wordNumber = evaluateWordNumber(dictArray[dictName]);
      unsigned int wordlength = evaluateAverageWordLength(dictArray[dictName]);
      nefedev::StreamGuard guard(out);
      out << std::fixed << std::setprecision(5);
      for (std::vector< WordAndFreq >::iterator iter = vect.begin(); iter != vect.end(); ++iter)
      {
        out << std::setw(wordlength) << iter->first << " - " << static_cast< double >(iter->second) / wordNumber << '\n';
      }
    }
    else
    {
      throw std::invalid_argument("INVALID ARGUMENT\n");
    }
  }
}

void nefedev::find(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    throw std::invalid_argument("Dictionary does not exist");
  }
  else
  {
    std::string input = "";
    in >> input;
    std::transform(input.begin(), input.end(), input.begin(), std::toupper);
    if (dictArray[dictName].find(input) != dictArray[dictName].end())
    {
      out << "TRUE\n";
    }
    else
    {
      out << "FALSE\n";
    }
  }
}

void nefedev::findAndPrint(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    throw std::invalid_argument("Dictionary does not exist");
  }
  else
  {
    std::string input;
    in >> input;
    std::transform(input.begin(), input.end(), input.begin(), std::toupper);
    if (dictArray[dictName].find(input) != dictArray[dictName].end())
    {
      unsigned int wordNumber = 0;
      wordNumber = evaluateWordNumber(dictArray[dictName]);
      nefedev::StreamGuard guard(out);
      out << std::fixed << std::setprecision(5);
      out << input << ' ' << static_cast< double >(dictArray[dictName][input]) / wordNumber << '\n';
    }
    else
    {
      out << "FALSE\n";
    }
  }
}

void nefedev::merge(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    throw std::invalid_argument("Dictionary does not exist");
  }
  else
  {
    std::string mergingDictName = "";
    std::cin >> mergingDictName;
    if (dictArray.find(mergingDictName) == dictArray.end())
    {
      throw std::invalid_argument("Merging dictionary does not exist");
    }
    else
    {
      for (FreqDictionary::iterator iter = dictArray[mergingDictName].begin(); iter != dictArray[mergingDictName].end(); ++iter)
      {
        insertWord(iter->first, dictArray[dictName]);
      }
      dictArray.erase(mergingDictName);
      out << "Dictionaries were merged successfully\n";
    }
  }
}

void nefedev::saveDict(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    throw std::invalid_argument("Dictionary does not exist");
  }
  else
  {
    std::string fileName;
    std::ofstream fout;
    in >> fileName;
    fout.open(fileName);
    fout << "SAVEDICT_FILE";
    for (FreqDictionary::iterator iter = dictArray[dictName].begin(); iter != dictArray[dictName].end(); ++iter)
    {
      fout << '\n' << iter->first << ' ' << iter->second;
    }
    out << "Dictionary was saved in file " << fileName << '\n';
  }
}

void nefedev::loadDict(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  std::string fileName;
  in >> fileName;
  std::ifstream fin(fileName);
  if (fin.is_open())
  {
    if (dictArray.find(dictName) != dictArray.end())
    {
      dictArray[dictName].clear();
    }
    else
    {
      FreqDictionary dict;
      dictArray.insert(std::make_pair(dictName, dict));
    }
    unsigned int wordNumber;
    std::string input;
    fin >> std::noskipws;
    using sep = nefedev::DelimeterIO;
    fin >> input;
    if (!fin || input != "SAVEDICT_FILE")
    {
      throw std::invalid_argument("INVALID FILE\n");
    }
    while (!fin.eof())
    {
      fin >> sep{ '\n' } >> input >> sep{ ' ' } >> wordNumber;
      if (!fin)
      {
        throw std::invalid_argument("Failed while reading the file\n");
      }
      dictArray[dictName].insert(std::make_pair(input, wordNumber));
    }
    if (dictArray[dictName].empty())
    {
      dictArray.erase(dictName);
      throw std::invalid_argument("Could not find any valid words");
    }
    else
    {
      out << "Dictionary was loaded from file " << fileName << '\n';
    }
  }
  else
  {
    throw std::invalid_argument("File does not exist");
  }
}
