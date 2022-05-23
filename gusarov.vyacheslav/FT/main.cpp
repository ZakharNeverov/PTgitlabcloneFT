#include <iostream>
#include <limits>
#include <string>
#include "CommandSystem.hpp"
#include "message.hpp"

int main()
{
  std::locale::global(std::locale(".utf8"));

  gusarov::Command list;
  while (!std::cin.eof()) {
    try {
      std::string commandName = "";
      std::cin >> commandName;
      if (!std::cin.eof()) {
        list.doCommand(commandName);
      }
    } catch (const std::invalid_argument&) {
      gusarov::printIncorrectInput(std::cout);
    } catch (const std::logic_error&) {
      gusarov::printIncompatibilityOfStructures(std::cout);
    }
  }
}
