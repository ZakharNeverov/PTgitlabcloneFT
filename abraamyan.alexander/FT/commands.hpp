#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <memory>
#include "table.hpp"

namespace abraamyan
{
  using tableArray = std::vector< Table >;
  void createCommand(tableArray &arr);
  void deleteCommand(tableArray &arr);
  void printCommand(tableArray &arr);
  void printToFileCommand(tableArray &arr);
  void readFileCommand(tableArray &arr);
  void unionCommand(tableArray &arr);
  void intersectionCommand(tableArray &arr);
  void searchCommand(tableArray &arr);
  void searchEverywhereCommand(tableArray &arr);
  void countCommand(tableArray &arr);
  void countEverywhereCommand(tableArray &arr);
  void copyCommand(tableArray &arr);
  void listCommand(tableArray &arr);
}

#endif
