#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "HaffmanStringUnion.hpp"
#include "Commands.hpp"
#include "Split.hpp"

int main()
{
  try
  {
    malakhov::Commands::StrsDictT dataset;
    malakhov::Commands commands{dataset, std::cout};

    std::string buf;
    std::string commandName;
    while (std::getline(std::cin, buf))
    {
      if (buf.empty())
      {
        continue;
      }
      malakhov::ForwardList< std::string > args;
      malakhov::split(buf, commandName, args);
      commands.call(commandName, args);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what();
    return 1;
  }
  return 0;
}
