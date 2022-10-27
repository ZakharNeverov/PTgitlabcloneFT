#include <iostream>
#include <limits>
#include <stdexcept>
#include <functional>
#include <string>
#include "commands.h"
#include "errorMessage.h"

int main()
{
  varnavskii::texts_t texts;
  varnavskii::dicts_t dictionaries;
  std::map< std::string, std::function< void() > > commandList(
   {
     {"add", std::bind(varnavskii::add, std::ref(texts), std::ref(dictionaries), std::ref(std::cin))},
     {"code", std::bind(varnavskii::code, std::ref(texts), std::ref(dictionaries), std::ref(std::cin))},
     {"encode", std::bind(varnavskii::encode, std::ref(texts), std::ref(dictionaries), std::ref(std::cin))},
     {"decode", std::bind(varnavskii::decode, std::ref(texts), std::ref(dictionaries), std::ref(std::cin))},
     {"merge", std::bind(varnavskii::merge, std::ref(texts), std::ref(dictionaries), std::ref(std::cin))},
     {"frequency", std::bind(varnavskii::frequency, std::ref(texts), std::ref(std::cin), std::ref(std::cout))},
     {"print", std::bind(varnavskii::print, std::ref(texts), std::ref(dictionaries), std::ref(std::cin), std::ref(std::cout))},
   });

  while (!std::cin.eof())
  {
    try
    {
      std::string commandName = "";
      if (std::cin >> commandName)
      {
        commandList.at(commandName)();
        if (std::cin.rdstate() == std::ios::failbit)
        {
          std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
          std::cin.clear();
          throw std::invalid_argument("incorrect input");
        }
      }
    }
    catch (const std::exception&)
    {
      varnavskii::printIncorrectInput(std::cerr);
      std::cerr << '\n';
    }
  }
}
