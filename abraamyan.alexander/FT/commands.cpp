#include "commands.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <string>

#include "ioctl.hpp"

namespace
{
  using namespace abraamyan;
  using arrIterator = std::vector< std::unique_ptr< abraamyan::Table > >::iterator;

  arrIterator getIteratorByName(std::string tableName);

  arrIterator getIterator(const std::string &tableName);

  void areArgumentsCorrect(arrIterator it1, arrIterator it2, std::string &newTableName);

  void readArg(std::string &arg);

  void readTwoArgs(std::string &arg1, std::string &arg2);

  void readThreeArgs(std::string &arg1, std::string &arg2, std::string &arg3);

  bool isBeginingExtraSymbol(char c);

  bool isEndingExtraSymbol(char c);

  bool isWord(std::string &str);

  bool removeExtraSymbols(std::string &str);

  void deleteTable(std::string &tableName);

  bool printTable(std::ostream &out, std::string &tableName);

  void listTables();

  bool isCreatable(std::string &tableName);

  bool createNewTable(std::string &tableName);

  arrIterator getIteratorByName(std::string tableName)
  {
    size_t i = 0;
    for (arrIterator first = arr.begin(); first != arr.end(); first++)
    {
      if ((*first)->getName() == tableName)
      {
        return first;
      }
    }
    return arr.end();
  }

  arrIterator getIterator(const std::string &tableName)
  {
    try
    {
      size_t i = std::stoul(tableName);
      if (i == 0)
      {
        throw std::logic_error("");
      }
      i--;
      return std::find(arr.begin(), arr.end(), arr[i]);
    }
    catch (const std::invalid_argument &e)
    {
      return getIteratorByName(tableName);
    }
  }

  void areArgumentsCorrect(arrIterator it1, arrIterator it2, std::string &newTableName)
  {
    if (it1 == arr.end() || it2 == arr.end() || it1 == it2 || !isCreatable(newTableName))
    {
      throw std::logic_error("");
    }
  }

  void readArg(std::string &arg)
  {
    std::string tmpArg;
    std::cin >> ArgIO{ tmpArg, true };
    if (!std::cin)
    {
      throw std::logic_error("");
    }
    arg = std::move(tmpArg);
  }

  void readTwoArgs(std::string &arg1, std::string &arg2)
  {
    std::string tmpArg1;
    std::cin >> ArgIO{ tmpArg1 };
    std::string tmpArg2;
    std::cin >> ArgIO{ tmpArg2 };
    if (!std::cin)
    {
      throw std::logic_error("");
    }
    arg1 = std::move(tmpArg1);
    arg2 = std::move(tmpArg2);
  }

  void readThreeArgs(std::string &arg1, std::string &arg2, std::string &arg3)
  {
    std::string tmpArg1;
    std::cin >> ArgIO{ tmpArg1 };
    std::string tmpArg2;
    std::cin >> ArgIO{ tmpArg2 };
    std::string tmpArg3;
    std::cin >> ArgIO{ tmpArg3 };
    if (!std::cin)
    {
      throw std::logic_error("");
    }
    arg1 = std::move(tmpArg1);
    arg2 = std::move(tmpArg2);
    arg3 = std::move(tmpArg3);
  }

  bool isBeginingExtraSymbol(char c)
  {
    return c == 40 || c == 91 || c == 123 || c == 44 || c == 34;
  }

  bool isEndingExtraSymbol(char c)
  {
    return c == 41 || c == 93 || c == 125 || c == 44 || c == 46 || c == 33 || c == 63 || c == 34 || c == 58 || c == 59;
  }

  bool isWord(std::string &str)
  {
    for (size_t i = 0; i < str.length(); i++)
    {
      if (!((str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122)))
      {
        return false;
      }
    }
    return true;
  }

  bool removeExtraSymbols(std::string &str)
  {
    while (str.length() != 0 && isBeginingExtraSymbol(str[0]))
    {
      str.erase(0, 1);
    }

    while (str.length() != 0 && isEndingExtraSymbol(str[str.length() - 1ul]))
    {
      str.erase(str.length() - 1ul, 1);
    }

    return str.length();
  }

  void deleteTable(std::string &tableName)
  {
    arrIterator it = getIterator(tableName);
    if (it == arr.end())
    {
      throw std::logic_error("");
    }
    arr.erase(it);
  }

  bool printTable(std::ostream &out, std::string &tableName)
  {
    arrIterator it = getIterator(tableName);
    if (it == arr.end())
    {
      throw std::logic_error("");
    }
    (*it)->printTable(out);
    return true;
  }

  void listTables()
  {
    if (arr.size() == 0)
    {
      std::cout << "Empty!\n";
      return;
    }
    for (size_t i = 0; i < arr.size(); i++)
    {
      std::cout << i + 1 << ".  " << arr[i]->getName() << '\n';
    }
    return;
  }

  bool isCreatable(std::string &tableName)
  {
    try
    {
      std::stoul(tableName);
      return false;
    }
    catch (const std::invalid_argument &e)
    {
      if (getIterator(tableName) != arr.end())
      {
        return false;
      }
      return true;
    }
  }

  bool createNewTable(std::string &tableName)
  {
    if (isCreatable(tableName))
    {
      arr.push_back(std::make_unique< Table >(tableName));
      return true;
    }
    return false;
  }

}

namespace abraamyan
{
  void performCommand()
  {
    std::string command;
    std::cin >> CommandIO{ command };
    if (!std::cin)
    {
      return;
    }
    if (command == "create")
    {
      std::string tableName;
      readArg(tableName);
      if (!createNewTable(tableName))
      {
        throw std::logic_error("");
      }
    }
    else if (command == "delete")
    {
      std::string tableName;
      readArg(tableName);
      deleteTable(tableName);
    }
    else if (command == "print")
    {
      std::string tableName;
      readArg(tableName);
      printTable(std::cout, tableName);
    }
    else if (command == "printToFile")
    {
      std::string fileName;
      std::string tableName;
      readTwoArgs(fileName, tableName);
      arrIterator it = getIterator(tableName);
      if (it == arr.end())
      {
        throw std::logic_error("");
      }
      std::ofstream out(fileName);
      if (!out.is_open())
      {
        throw std::logic_error("");
      }
      (*it)->printTable(out);
      out.close();
    }
    else if (command == "readFile")
    {
      std::string fileName;
      std::string tableName;
      readTwoArgs(fileName, tableName);
      bool isTableExists = getIterator(tableName) != arr.end();
      if (!isTableExists && !createNewTable(tableName))
      {
        throw std::logic_error("");
      }
      std::ifstream inputFile(fileName);
      if (!inputFile.is_open())
      {
        throw std::logic_error("");
      }
      size_t lineNum = 1ul;
      while (!inputFile.eof())
      {
        while (!inputFile.eof() && std::isspace(inputFile.peek()) && inputFile.peek() != '\n')
        {
          inputFile.ignore(1);
        }
        if (inputFile.eof() || inputFile.peek() == '\n')
        {
          lineNum++;
          inputFile.ignore(1);
          continue;
        }
        std::string word;
        inputFile >> word;
        if (!inputFile)
        {
          std::cout << "Fatal error occurred reading file!\n";
          return;
        }
        if (removeExtraSymbols(word) && isWord(word))
        {
          (*(getIterator(tableName)))->insert(word, lineNum);
        }
      }
      inputFile.close();
    }
    else if (command == "union")
    {
      std::string tableName1;
      std::string tableName2;
      std::string newTableName;
      readThreeArgs(tableName1, tableName2, newTableName);
      arrIterator it1 = getIterator(tableName1);
      arrIterator it2 = getIterator(tableName2);
      areArgumentsCorrect(it1, it2, newTableName);
      arr.push_back(std::make_unique< Table >((*it1)->unite(**(it2), newTableName)));
    }
    else if (command == "intersection")
    {
      std::string tableName1;
      std::string tableName2;
      std::string newTableName;
      readThreeArgs(tableName1, tableName2, newTableName);
      arrIterator it1 = getIterator(tableName1);
      arrIterator it2 = getIterator(tableName2);
      areArgumentsCorrect(it1, it2, newTableName);
      arr.push_back(std::make_unique< Table >((*it1)->intersect(**(it2), newTableName)));
    }
    else if (command == "list")
    {
      listTables();
    }
    else if (command == "search")
    {
      std::string word;
      std::string tableName;
      readTwoArgs(word, tableName);
      arrIterator it = getIterator(tableName);
      if (it == arr.end())
      {
        throw std::logic_error("");
      }
      std::cout << ((*it)->find(word)) ? "Found!\n" : "Wasnt found\n";
    }
    else if (command == "searchEverywhere")
    {
      std::string word;
      readArg(word);
      for (size_t i = 0; i < arr.size(); i++)
      {
        if (arr[i]->find(word))
        {
          std::cout << "Found!\n";
          return;
        }
      }
      std::cout << "Wasnt found!\n";
    }
    else if (command == "count")
    {
      std::string word;
      std::string tableName;
      readTwoArgs(word, tableName);
      arrIterator it = getIterator(tableName);
      if (it == arr.end())
      {
        throw std::logic_error("");
      }
      std::cout << (*it)->count(word) << '\n';
    }
    else if (command == "countEverywhere")
    {
      std::string word;
      readArg(word);
      size_t count = 0;
      for (auto &it : arr)
      {
        count += it->count(word);
      }
      std::cout << count << '\n';
    }
    else if (command == "copyTable")
    {
      std::string tableName;
      std::string tableName2;
      readTwoArgs(tableName, tableName2);
      arrIterator it = getIterator(tableName);
      if (it == arr.end() || !isCreatable(tableName2))
      {
        throw std::logic_error("");
      }
      arr.push_back(std::make_unique< Table >(Table(**it)));
      arr[arr.size() - 1]->setName(tableName2);
    }
    else
    {
      throw std::logic_error("");
    }
  }
}
