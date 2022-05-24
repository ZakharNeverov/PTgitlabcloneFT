#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include "CommandWrappers.h"
#include "ErrorMessages.h"
#include "IoProcessing.h"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Error: ";
    borisov::printInvalidNumberOfArguments(std::cerr) << '\n';
    return 1;
  }

  std::ifstream fileIn(argv[1]);
  if (!fileIn)
  {
    std::cerr << "Error: ";
    borisov::printFailToOpenFile(std::cerr) << '\n';
    return 1;
  }

  borisov::AllDicts allDicts;
  while (!fileIn.eof())
  {
    try
    {
      std::string name;
      fileIn >> name;
      bool s = allDicts.insert({name, borisov::Dict()}).second;
      if (!s)
      {
        throw std::invalid_argument("Invalid file format");
      }
      borisov::Dict& dict = allDicts[name];
      borisov::getDict(fileIn, dict);
      if (fileIn.fail() && !fileIn.eof())
      {
        throw std::invalid_argument("Invalid file format");
      }
    }
    catch (const std::invalid_argument& e)
    {
      borisov::printInvalidFileFormat(std::cerr);
      return 1;
    }
    catch (const std::exception& e)
    {
      std::cerr << "Error: " << e.what() << '\n';
      return 2;
    }
  }
  fileIn.close();

  using namespace std::placeholders;
  std::map< std::string, std::function< void(const borisov::ArgList&) > > commands
  {
    {"add", std::bind(borisov::addWrapper, std::ref(allDicts), _1)},
    {"remove", std::bind(borisov::removeWrapper, std::ref(allDicts), _1)},
    {"manual", std::bind(borisov::manualWrapper, std::ref(allDicts), _1)},
    {"words", std::bind(borisov::wordsWrapper, std::ref(std::cout), std::ref(allDicts), _1)},
    {"info", std::bind(borisov::infoWrapper, std::ref(std::cout), std::ref(allDicts), _1)},
    {"translate", std::bind(borisov::translateWrapper, std::ref(std::cout), std::ref(allDicts), _1)},
    {"save", std::bind(borisov::saveWrapper, std::ref(allDicts), _1)},
    {"union", std::bind(borisov::uniteWrapper, std::ref(allDicts), _1)},
    {"intersect", std::bind(borisov::intersectWrapper, std::ref(allDicts), _1)},
    {"complement", std::bind(borisov::complementWrapper, std::ref(allDicts), _1)}
  };

  std::string s;
  while (!std::cin.eof() && !(std::cin.peek() && std::cin.eof()))
  {
    try
    {
      borisov::ArgList l;
      borisov::getArgList(std::cin, l);
      if (std::cin.fail() && !std::cin.eof() || l.size() == 0)
      {
        throw std::invalid_argument("Invalid command");
      }
      auto commandIter = commands.find(l.front());
      if (commandIter == commands.end())
      {
        throw std::invalid_argument("Invalid command");
      }
      l.pop_front();
      commandIter->second(l);
    }
    catch (const std::invalid_argument& e)
    {
      borisov::printInvalidCommandMessage(std::cout) << '\n';
      if (std::cin.fail() && !std::cin.eof())
      {
        borisov::restoreIStream(std::cin);
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << "Error: " << e.what() << '\n';
      return 2;
    }
  }
}
