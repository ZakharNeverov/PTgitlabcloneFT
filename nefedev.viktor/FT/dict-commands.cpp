#include "dict-commands.hpp"
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <numeric>
#include <algorithm>
#include "stream-guard.hpp"

using FreqDictionary = std::map< std::string, unsigned int >;
using DictionaryArray = std::unordered_map< std::string, FreqDictionary >;
using WordAndFreq = std::pair< std::string, unsigned int >;

namespace
{
  std::string& deleteWrongChars(std::string& str)
  {
    for (size_t i = 0; i < str.length(); ++i)
    {
      if ((str[i] < '0' && str[i] != '-' && str[i] != '\'') || (str[i] > '9' && str[i] < 'A') || (str[i] > 'Z' && str[i] < 'a') || str[i] > 'z')
      {
        str.erase(i, 1);
        --i;
      }
    }
    if (!str.empty() && (str[0] == '-' || str[0] == '\''))
    {
      str.erase(0, 1);
    }
    if (!str.empty() && (str[str.length() - 1] == '-' || str[str.length() - 1] == '\''))
    {
      str.erase(str.length() - 1, 1);
    }
    return str;
  }
  bool compAscendingFreq(WordAndFreq element1, WordAndFreq element2)
  {
    return element1.second > element2.second;
  }
  bool compDescendingFreq(WordAndFreq element1, WordAndFreq element2)
  {
    return element1.second < element2.second;
  }
  unsigned int accumulateDictSum(unsigned int sum, WordAndFreq record)
  {
    return sum + record.second;
  }
  void insertWord(std::string word, FreqDictionary& dict)
  {
    deleteWrongChars(word);
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
}

void nefedev::insert(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  std::string word = "";
  in >> dictName;
  in >> word;
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  if (dictArray.find(dictName) == dictArray.end())
  {
    FreqDictionary dict;
    dictArray.insert(std::make_pair(dictName, dict));
  }
  insertWord(word, dictArray[dictName]);
  out << "Inserted word \"" << word << "\" to dictionary " << dictName << '\n';
}

void nefedev::read(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string input = "";
  in >> input;
  if (input == "FILE")
  {
    std::string dictName = "";
    in >> dictName;
    if (dictArray.find(dictName) == dictArray.end())
    {
      FreqDictionary dict;
      dictArray.insert(std::make_pair(dictName, dict));
    }
    std::ifstream fin;
    std::string fileName;
    in >> fileName;
    fin.open(fileName);
    while (!fin.eof())
    {
      fin >> input;
      insertWord(input, dictArray[dictName]);
    }
    out << "Inserted all words from file \"" << fileName << "\"\n";
  }
  else if (input == "TEXT")
  {
    std::string dictName = "";
    in >> dictName;
    if (dictArray.find(dictName) == dictArray.end())
    {
      FreqDictionary dict;
      dictArray.insert(std::make_pair(dictName, dict));
    }
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
    out << "Inserted all words from the text\n";
  }
}

void nefedev::deleteWord(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    out << "There is no such dictionary\n";
  }
  else
  {
    std::string input = "";
    in >> input;
    if (dictArray[dictName].find(input) == dictArray[dictName].end())
    {
      std::cout << "There is no word \"" << input << "\" in the dictionary with number " << dictName << '\n';
    }
    else if (dictArray[dictName][input] > 1)
    {
      --dictArray[dictName][input];
      out << "Deleted 1 enterance of the word from dictionary\n";
    }
    else
    {
      dictArray[dictName].erase(input);
      out << "Deleted 1 enterance of the word from dictionary\n";
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
    out << "There is no such dictionary\n";
  }
  else
  {
    std::string input = "";
    in >> input;
    if (dictArray[dictName].find(input) == dictArray[dictName].end())
    {
      std::cout << "There is no word \"" << input << "\" in the dictionary with number " << dictName << '\n';
    }
    else
    {
      dictArray[dictName].erase(input);
      out << "Deleted all enterances of the word\n";
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
    out << "There is no such dictionary\n";
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
    out << "There is no such dictionary\n";
  }
  else
  {
    std::string order = "";
    std::getline(in, order);
    order.erase(0, 1);
    if (order == "ASCENDING" || order == "")
    {  
      unsigned int wordNumber = 0;
      wordNumber = evaluateWordNumber(dictArray[dictName]);
      nefedev::StreamGuard guard(out);
      out << std::fixed << std::setprecision(5);
      for (FreqDictionary::iterator iter = dictArray[dictName].begin(); iter != dictArray[dictName].end(); ++iter)
      {
        out << std::setw(15) << iter->first << " - " << static_cast<double>(iter->second) / wordNumber << '\n';
      }
    }
    else if (order == "DESCENDING")
    {
      unsigned int wordNumber = 0;
      wordNumber = evaluateWordNumber(dictArray[dictName]);
      nefedev::StreamGuard guard(out);
      out << std::fixed << std::setprecision(5);
      for (FreqDictionary::reverse_iterator iter = dictArray[dictName].rbegin(); iter != dictArray[dictName].rend(); ++iter)
      {
        out << std::setw(15) << iter->first << " - " << static_cast<double>(iter->second) / wordNumber << '\n';
      }
    }
    else
    {
      out << "INVALID ARGUMENT\n";
    }
  }
}

void nefedev::printFrequency(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    out << "There is no such dictionary\n";
  }
  else
  {
    std::vector< WordAndFreq > vect(dictArray[dictName].begin(),dictArray[dictName].end());
    std::string order;
    std::getline(in, order);
    order.erase(0, 1);
    if (order == "ASCENDING" || order == "")
    {
      std::sort(vect.begin(), vect.end(), compAscendingFreq);
      unsigned int wordNumber = 0;
      wordNumber = evaluateWordNumber(dictArray[dictName]);
      nefedev::StreamGuard guard(out);
      out << std::fixed << std::setprecision(5);
      for (std::vector< WordAndFreq >::iterator iter = vect.begin(); iter != vect.end(); ++iter)
      {
        out << std::setw(15) << iter->first << " - " << static_cast<double>(iter->second) / wordNumber << '\n';
      }
    }
    else if (order == "DESCENDING")
    {
      std::sort(vect.begin(), vect.end(), compDescendingFreq);
      unsigned int wordNumber = 0;
      wordNumber = evaluateWordNumber(dictArray[dictName]);
      nefedev::StreamGuard guard(out);
      out << std::fixed << std::setprecision(5);
      for (std::vector< WordAndFreq >::iterator iter = vect.begin(); iter != vect.end(); ++iter)
      {
        out << std::setw(15) << iter->first << " - " << static_cast<double>(iter->second) / wordNumber << '\n';
      }
    }
    else
    {
      out << "INVALID ARGUMENT\n";
    }
  }
}

void nefedev::find(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    out << "There is no such dictionary\n";
  }
  else
  {
    std::string input = "";
    in >> input;
    if (dictArray[dictName].find(input) != dictArray[dictName].end())
    {
      out << "True\n";
    }
    else
    {
      out << "False\n";
    }
  }
}

void nefedev::findAndPrint(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    out << "There is no such dictionary\n";
  }
  else
  {
    std::string input;
    in >> input;
    if (dictArray[dictName].find(input) != dictArray[dictName].end())
    {
      unsigned int wordNumber = 0;
      wordNumber = evaluateWordNumber(dictArray[dictName]);
      nefedev::StreamGuard guard(out);
      out << std::fixed << std::setprecision(5);
      out << std::setw(15) << input << ' ' << static_cast<double>(dictArray[dictName][input]) / wordNumber << '\n';
    }
    else
    {
      out << "False\n";
    }
  }
}

void nefedev::merge(std::istream& in, std::ostream& out, DictionaryArray& dictArray)
{
  std::string dictName = "";
  in >> dictName;
  if (dictArray.find(dictName) == dictArray.end())
  {
    out << "Dictionary " << dictName << " does not exist\n";
  }
  else
  {
    std::string mergingDictName = "";
    std::cin >> mergingDictName;
    if (dictArray.find(mergingDictName) == dictArray.end())
    {
      out << "Dictionary " << mergingDictName << " does not exist\n";
    }
    else
    {
      for (FreqDictionary::iterator iter = dictArray[mergingDictName].begin(); iter != dictArray[mergingDictName].end(); ++iter)
      {
        insertWord(iter->first, dictArray[dictName]);
      }
      dictArray.erase(mergingDictName);
    }
  }
}
