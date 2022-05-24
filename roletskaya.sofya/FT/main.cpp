#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <functional>
#include "Commands.hpp"
#include "Dictionary.hpp"
#include "Utilites.hpp"
#include "WrappersForCommands.hpp"

using map = std::map< std::string, std::list< std::string > >;
using dictsArray = std::map< std::string, roletskaya::Dictionary >;

int main(int argc, char* argv[])
{
  if (argc <= 2)
  {
    std::cerr << "Too few files.\n";
    return 1;
  }
  std::string commandFileName = argv[1];

  std::ifstream in;
  std::ofstream out;
  std::string outFileName = "";
  std::string checkFile = "";
  std::cout << "Enter file's name to output results: ";
  std::cin >> outFileName;
  std::cout << "\nEnter file's name to compare results: ";
  std::cin >> checkFile;

  dictsArray dicts;
  using command_t = std::function< void(std::string& line) >;
  std::map< std::string, command_t > commands({
    {"insert", std::bind(roletskaya::insert, std::placeholders::_1, std::ref(dicts), std::ref(out))},
    {"findWordByTranslation", std::bind(roletskaya::findWordByTranslation, std::placeholders::_1, std::ref(dicts), std::ref(out))},
    {"findTranslation", std::bind(roletskaya::findTranslation,  std::placeholders::_1, std::ref(dicts), std::ref(out))},
    {"translateText", std::bind(roletskaya::translateText,  std::placeholders::_1, std::ref(dicts), std::ref(out))},
    {"delete", std::bind(roletskaya::deleteKey,  std::placeholders::_1, std::ref(dicts), std::ref(out))},
    {"print", std::bind(roletskaya::print,  std::placeholders::_1, std::ref(dicts), std::ref(out))},
    {"merge", std::bind(roletskaya::doMerge, std::placeholders::_1, std::ref(dicts), std::ref(out))},
    {"complement", std::bind(roletskaya::doComplement, std::placeholders::_1, std::ref(dicts), std::ref(out))},
    {"equals", std::bind(roletskaya::ifEquals, std::placeholders::_1, std::ref(dicts), std::ref(out))}
  });

  try
  {
    for (int i = 2; i < argc; ++i)
    {
      std::string fileName = argv[i];
      std::string dictName = roletskaya::getNameFromFile(fileName);
      if (roletskaya::pushDictToArray(dictName, fileName, dicts) == false)
      {
        return 1;
      }
    }

    in.open(commandFileName);
    if (!in)
    {
      throw std::invalid_argument("Couldn't open the input file.\n");
    }
    out.open(outFileName);
    if (!out)
    {
      throw std::invalid_argument("Couldn't open the output file.\n");
    }
    while (!in.eof())
    {
      try
      {
        std::string command = "";
        std::string line = "";
        std::getline(in, line);
        command = roletskaya::getElem(line);
        if (!command.empty())
        {
          auto command_iter = commands.find(command);
          if (command_iter == commands.end())
          {
            throw std::invalid_argument("No such command.\n");
          }
          command_iter->second(line);
        }      
      }
      catch (const std::exception& error)
      {
        out << error.what();
      }
    }
    in.close();
    out.close();
    if (roletskaya::checkResults(outFileName, checkFile) == false)
    {
      throw std::logic_error("Results are not correct.\n");
    }
    else
    {
      std::cout << "Results are correct.\n";
    }
  }
  catch (const std::exception& error)
  {
    std::cerr << error.what() << '\n';
    return 1;
  }
  return 0;
}
