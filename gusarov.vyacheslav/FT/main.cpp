#include <iostream>
#include <map>
#include <limits>
#include <string>
#include "CommandSystem.hpp"

int main()
{
  gusarov::Command list;
  while (!std::cin.eof()) {
    try {
      std::string commandName = "";
      std::cin >> commandName;
      if (!std::cin.eof()) {
        list.doCommand(commandName);
      }
    } catch (const std::invalid_argument& err) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cerr << err.what() << '\n';
    }
  }
}
