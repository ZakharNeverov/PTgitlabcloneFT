#include <fstream>
#include <iostream>
#include <map>
#include "Commands.hpp"
#include "DataParse.hpp"
#include "printInformation.hpp"
int main(int argc, char **argv)
{
  if (argc != 2) {
    matyushenko::printBadNumberOfArgs(std::cerr);
    return 1;
  }
  try {
    std::map <std::string, std::string> lists;
    std::ifstream input{argv[1]};
    if (!input.is_open()) {
      matyushenko::printUnableToReadFile(std::cerr);
      return 1;
    }
    std::string strBuf;
    while (std::getline(input, strBuf)) {
      if (strBuf.empty()) {
        continue;
      }
      const std::string listName = matyushenko::getPartOfString(strBuf);
      lists.insert({listName, {}});
      auto &posToActWith = lists[listName];
      while (!strBuf.empty()) {
        posToActWith = (matyushenko::getPartOfString(strBuf));
      }
    }
    using Commands = matyushenko::Commands;
    Commands commands{lists, std::cout};
    std::string commandsBuf;
    while (std::getline(std::cin, commandsBuf)) {
      if (commandsBuf.empty()) {
        continue;
      }
      const std::string commandName = matyushenko::getPartOfString(commandsBuf);
      Commands::ListOfArgs args;
      while (!commandsBuf.empty()) {
        std::string arg = matyushenko::getPartOfString(commandsBuf);
        args.push_back(arg);
      }
      commands.call(commandName, args);
    }
  } catch (const std::exception &e) {
    std::cerr << e.what();
    return 1;
  }
  return 0;
}
