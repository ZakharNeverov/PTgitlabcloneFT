#include <iostream>
#include <limits>
#include <string>
#include "CommandSystem.hpp"
#include "message.hpp"

int main()
{
  gusarov::Command list;
  while (!std::cin.eof()) {
    try {
      std::string commandName = "";
      if (std::cin >> commandName) {
        list.doCommand(commandName);
      }
    } catch (const std::exception&) {
      gusarov::printIncorrectInput(std::cout);
    }
  }
}

