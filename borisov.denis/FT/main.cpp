#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include "CommandWrappers.h"
#include "ErrorMessages.h"
#include "StringUtils.h"

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
  std::string s;
  while (!fileIn.eof())
  {
    std::getline(fileIn, s);
    if (!s.empty())
    {
      try
      {
        std::list< std::string > l;
        borisov::utils::split(s, l);
        std::string name = l.front();
        l.pop_front();
        bool s = allDicts.insert({name, borisov::Dict()}).second;
        if (!s)
        {
          throw std::invalid_argument("Invalid file format");
        }
        borisov::Dict& dict = allDicts[name];
        auto iter = l.cbegin();
        auto iterEnd = l.cend();
        while (iter != iterEnd)
        {
          const borisov::Word& word = *(iter++);
          if (iter == iterEnd)
          {
            throw std::invalid_argument("Invalid file format");
          }
          const borisov::Translation& translation = *(iter++);
          if (iter == iterEnd)
          {
            throw std::invalid_argument("Invalid file format");
          }
          size_t count = std::stoull(*(iter++));
          borisov::add(dict, word, translation, count);
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

  while (!std::cin.eof() && !(std::cin.peek() && std::cin.eof()))
  {
    std::getline(std::cin, s);
    if (!s.empty())
    {
      try
      {
        borisov::ArgList l;
        borisov::utils::split(s, l);
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
      }
      catch (const std::exception& e)
      {
        std::cerr << "Error: " << e.what() << '\n';
        return 2;
      }
    }
  }
}
