#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <memory>
#include "table.hpp"

namespace abraamyan
{
  static std::vector< std::unique_ptr< Table > > arr;
  void performCommand();
}

#endif
