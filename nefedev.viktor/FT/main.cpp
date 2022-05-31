#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <functional>
#include "dict-commands.hpp"

int main()
{
  std::unordered_map<std::string, std::map< std::string, unsigned int > > dictArray;
  std::string input = "";
  std::map< std::string, std::function< void(void) > > commands(
    {
      {"INSERT", std::bind(nefedev::insert, std::ref(std::cin), std::ref(std::cout), std::ref(dictArray))},
      {"READTEXT", std::bind(nefedev::readText, std::ref(std::cin), std::ref(std::cout), std::ref(dictArray))},
      {"READFILE", std::bind(nefedev::readFile, std::ref(std::cin), std::ref(std::cout), std::ref(dictArray))},
      {"DELETE", std::bind(nefedev::deleteWord, std::ref(std::cin), std::ref(std::cout), std::ref(dictArray))},
      {"DELETEALL", std::bind(nefedev::deleteAllWords, std::ref(std::cin), std::ref(std::cout), std::ref(dictArray))},
      {"WORDNUMBER", std::bind(nefedev::wordNumber, std::ref(std::cin), std::ref(std::cout), std::ref(dictArray))},
      {"PRINTALPHABETIC", std::bind(nefedev::printAlphabetic, std::ref(std::cin), std::ref(std::cout), std::ref(dictArray))},
      {"PRINTFREQUENCY", std::bind(nefedev::printFrequency, std::ref(std::cin), std::ref(std::cout), std::ref(dictArray))},
      {"FIND", std::bind(nefedev::find, std::ref(std::cin), std::ref(std::cout), std::ref(dictArray))},
      {"FINDANDPRINT", std::bind(nefedev::findAndPrint, std::ref(std::cin), std::ref(std::cout), std::ref(dictArray))},
      {"MERGE", std::bind(nefedev::merge, std::ref(std::cin), std::ref(std::cout), std::ref(dictArray))},
      {"SAVEDICT", std::bind(nefedev::saveDict, std::ref(std::cin), std::ref(std::cout), std::ref(dictArray))},
      {"LOADDICT", std::bind(nefedev::loadDict, std::ref(std::cin), std::ref(std::cout), std::ref(dictArray))},
    });
  while (!std::cin.eof())
  {
    input = "";
    std::cin >> input;
    if (!input.empty())
    {
      try
      {
        commands.at(input)();
      }
      catch (const std::exception& e)
      {
        nefedev::outInvalidCommand(std::cout);
        nefedev::cleanIStream(std::cin);
      }
    }
  }
  return 0;
}
