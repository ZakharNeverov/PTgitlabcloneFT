#include <iostream>
#include <fstream>
#include <functional>
#include <locale>
#include "dicts.hpp"
#include "commands.hpp"

int main(int argc, char** argv)
{
  std::locale local = std::locale::global(std::locale("ru_RU.utf8"));
  if (argc != 2)
  {
    std::cerr << "Bad args!\n";
    return 1;
  }
  std::ifstream fin(argv[1]);
  if (!fin.is_open())
  {
    std::cerr << "File not open!\n";
    return 1;
  }
  bokov::dicts_list dicts;
  while (!fin.eof())
  {
    try
    {
      fin.clear();
      std::string name;
      std::string file;
      fin >> name >> file;
      bokov::dictionary dictBuf;
      bokov::makeDictionary(file, dictBuf);
      dicts[name] = dictBuf;
    }
    catch (std::exception& err)
    {
      std::cerr << err.what() << '\n';
      fin.clear();
      fin.ignore(std::numeric_limits < std::streamsize >::max(), '\n');
    }
  }
  using namespace std::placeholders;
  using dict_func = std::function < void(std::string&) >;
  std::map < std::string, dict_func > commands
  {
    {"print", std::bind(&bokov::printDict, _1, std::ref(dicts), std::ref(std::cout))},
    {"load", std::bind(&bokov::loadDict, _1 , std::ref(dicts))},
    {"remove", std::bind(&bokov::removeElement, _1, std::ref(dicts))},
    {"merge", std::bind(&bokov::mergeDicts, _1, std::ref(dicts))},
    {"intersect", std::bind(&bokov::intersectDicts, _1, std::ref(dicts))},
    {"complement", std::bind(&bokov::complementDicts, _1, std::ref(dicts))},
    {"top", std::bind(&bokov::getTop, _1, std::ref(dicts), std::ref(std::cout))},
    {"insert", std::bind(&bokov::insertInDict, _1, std::ref(dicts))},
    {"search", std::bind(&bokov::searchInDict, _1, std::ref(dicts), std::ref(std::cout))},
    {"delete", std::bind(&bokov::deleteDict, _1, std::ref(dicts))}
      };
  while (!std::cin.eof())
  {
    std::cin.clear();
    std::string commandName;
    std::cin >> commandName;
    if (!commandName.empty())
    {
      try
      {
        auto commandFunc = commands.find(commandName);
        if (commandFunc == commands.end())
        {
          throw std::invalid_argument("Incorrect command taken!");
        }
        std::string command;
        std::getline(std::cin, command);
        while (command.front() == ' ')
        {
          command.erase(0, 1);
        }
        while (command.back() == ' ')
        {
          command.pop_back();
        }
        commandFunc->second(command);
      }
      catch (std::exception& err)
      {
        std::cerr << err.what() << '\n';
      }
    }
  }
}