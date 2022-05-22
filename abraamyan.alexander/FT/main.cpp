#include <iostream>
#include <limits>
#include "table.hpp"
#include "commands.hpp"

int main()
{
  while (!std::cin.eof())
  {
    try
    {
      abraamyan::performCommand();
    }
    catch (const std::logic_error &e)
    {
      std::cout << "<INVALID COMMAND>\n";
      if (!std::cin.eof())
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }
  return 0;
}
