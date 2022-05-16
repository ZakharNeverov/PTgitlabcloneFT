#include <iostream>
#include <iomanip>
#include <string>
#include <functional>
#include <list>
#include <fstream>
#include "printMessages.h"
#include "StreamProcessing.h"
#include "commandWrappers.h"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    dudnik::printBadNumberOfArgs(std::cerr);
    return 1;
  }
  std::ifstream fin(argv[1]);
  if (!fin.good())
  {
    dudnik::printUnableToReadFile(std::cerr);
    return 1;
  }
  try
  {
    auto data = dudnik::getDictsFromStream(fin);
    fin.close();
    std::map< std::string, std::function< void(dudnik::Args&, dudnik::AllDicts&) > > commands
    {
      {"print", dudnik::printWrapper},
      {"complement", dudnik::complementWrapper},
      {"intersect", dudnik::intersectWrapper},
      {"union", dudnik::uniteWrapper},
      {"remove", dudnik::removeWrapper},
      {"replace", dudnik::replaceWrapper},
      {"equal", dudnik::equalWrapper},
      {"insert", dudnik::insertWrapper},
      {"clear", dudnik::clearWrapper},
      {"add", dudnik::addWrapper},
      {"save", dudnik::saveWrapper}
    };
    while (!std::cin.eof())
    {
      try
      {
        auto words = dudnik::getWords(std::cin, '\n');
        if (words.empty())
        {
          continue;
        }
        auto commandIt = commands.find(words.front());
        if (commandIt == commands.end())
        {
          throw std::invalid_argument("This command is not supported");
        }
        words.pop_front();
        commandIt->second(words, data);
      }
      catch (const std::invalid_argument&)
      {
        dudnik::printInvalidCommand(std::cout);
      }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 2;
  }
  return 0;
}
