#include "Commands.h"
#include <map>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <functional>
#include "IoOperations.h"
#include "AdditionalCommands.h"

namespace kolosovskaya
{
  void doPrint(const Dicts& allDicts, std::istream& in, std::ostream& out)
  {
    std::string dicts = "";
    in >> dicts;
    if (dicts == "all")
    {
      for (auto const& dict : allDicts)
      {
        printDict(out, dict.first, allDicts);
      }
    }
    else
    {
      printDict(out, dicts, allDicts);
    }
  }

  void doComplement(Dicts& allDicts, std::istream& in, std::ostream& out)
  {
    std::string str = "";
    std::getline(in, str);
    std::vector< std::string > dictNames = getNamesFromString(str, ' ');
    if (dictNames.size() != 3)
    {
      printWrongArgsMessage(out);
      return;
    }
    auto namesIter = dictNames.begin();
    std::string newDictName = *namesIter;
    namesIter = dictNames.erase(namesIter);
    if (!doesExist(newDictName, allDicts))
    {
      for (auto&& name : dictNames)
      {
        if (!doesExist(name, allDicts))
        {
          printNotExistedDictMessage(out);
          return;
        }
      }
    }
    else
    {
      printExistedDictMessage(out);
      return;
    }
    auto first = allDicts.find(*(namesIter++));
    auto second = allDicts.find(*(namesIter++));
    Dict newDict = complementDicts(first->second, second->second);
    allDicts[newDictName] = newDict;
  }

  void doUnion(Dicts& allDicts, std::istream& in, std::ostream& out)
  {
    std::string str = "";
    std::getline(in, str);
    std::vector< std::string > dictNames = getNamesFromString(str, ' ');
    if (dictNames.size() != 3)
    {
      printWrongArgsMessage(out);
      return;
    }
    auto namesIter = dictNames.begin();
    std::string newDictName = *namesIter;
    namesIter = dictNames.erase(namesIter);
    if (!doesExist(newDictName, allDicts))
    {
      for (auto&& name : dictNames)
      {
        if (!doesExist(name, allDicts))
        {
          printNotExistedDictMessage(out);
          return;
        }
      }
    }
    else
    {
      printExistedDictMessage(out);
      return;
    }
    auto first = allDicts.find(*(namesIter++));
    auto second = allDicts.find(*(namesIter++));
    Dict newDict = makeUnion(first->second, second->second);
    allDicts[newDictName] = newDict;
  }

  void doLoad(const Dicts& allDicts, std::istream& in, std::ostream& out)
  {
    std::string dictName = "";
    in >> dictName;
    if (!doesExist(dictName, allDicts))
    {
      printNotExistedDictMessage(out);
      return;
    }
    std::string fileName = dictName + ".txt";
    std::ofstream fout(fileName);
    printDict(fout, dictName, allDicts);
    fout.close();
    out << "The dictionary " << dictName << " was successfully added!\n";
  }

  void doViewDicts(const Dicts& allDicts, std::istream& in, std::ostream& out)
  {
    bool isFirst = true;
    for (auto&& name : allDicts)
    {
      if (isFirst)
      {
        out << name.first;
      }
      else
      {
        out << ' ' << name.first;
      }
      isFirst = false;
    }
    out << '\n';
  }

  void doTranslate(Dicts& allDicts, std::istream& in, std::ostream& out)
  {
    std::string str = "";
    std::getline(in, str);
    std::vector< std::string > strArgs = getNamesFromString(str, ' ');
    if (!doesExist(strArgs[0], allDicts))
    {
      printNotExistedDictMessage(out);
      return;
    }
    const kolosovskaya::Dict& dict = allDicts[strArgs.at(0)];
    std::string word = strArgs.at(1);
    auto dictIt = dict.begin();
    while (dictIt != dict.end())
    {
      if ((dictIt)->first == word)
      {
        printRuWords(out, dictIt->second);
        out << '\n';
        return;
      }
      dictIt++;
    }
    out << "The dictionary " << strArgs.at(0) << " doesn't contain the word " << word << '\n';
  }

  void doAdd(Dicts& allDicts, std::istream& in, std::ostream& out)
  {
    std::string str1 = "";
    std::getline(in, str1);
    std::vector< std::string > strArgs = getNamesFromString(str1, ' ');
    if (strArgs.size() != 2)
    {
      out << "You have to enter only dictName and enWord!\n";
      return;
    }
    if (!doesExist(strArgs[0], allDicts))
    {
      printNotExistedDictMessage(out);
      return;
    }
    kolosovskaya::Dict& dict = allDicts[strArgs.at(0)];
    std::string word = strArgs.at(1);
    out << "Add translation of the word " << word << '\n';
    std::string str2 = "";
    std::getline(in, str2);
    std::vector< std::string > vector = getNamesFromString(str2, ',');
    std::set<std::string> set(vector.begin(), vector.end());
    dict[word] = set;
    out << '\n';
  }
}
