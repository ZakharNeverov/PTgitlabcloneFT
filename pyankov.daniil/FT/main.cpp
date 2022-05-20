#include <iostream>
#include <functional>
#include <map>
#include <stdexcept>
#include <string>
#include <limits>
#include "matrix.hpp"
#include "commands.hpp"
#include "print-messages.hpp"
int main()
{
  using matrices_t = std::map< std::string, pyankov::Matrix< double > >;
  matrices_t matrices;
  std::map< std::string, std::function< void() > > commands(
    {
      {"create", std::bind(pyankov::create, std::ref(matrices), std::ref(std::cin))},
      {"multiply", std::bind(pyankov::multiply, std::ref(matrices), std::ref(std::cin))},
      {"add", std::bind(pyankov::add, std::ref(matrices), std::ref(std::cin))},
      {"sub", std::bind(pyankov::sub, std::ref(matrices), std::ref(std::cin))},
      {"concat", std::bind(pyankov::concat, std::ref(matrices), std::ref(std::cin))},
      {"print", std::bind(pyankov::print, std::ref(matrices), std::ref(std::cin), std::ref(std::cout))},
      {"equals", std::bind(pyankov::equals, std::ref(matrices), std::ref(std::cin), std::ref(std::cout))}
    }
  );
  while (!std::cin.eof())
  {
    std::string mainCommandName = "";
    std::cin >> mainCommandName;
    if (mainCommandName.empty())
    {
      continue;
    }
    try
    {
      commands.at(mainCommandName)();
    }
    catch (const std::exception& ex)
    {
      pyankov::printInvalidCommand(std::cout);
      if (std::cin.rdstate() == std::ios::failbit)
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }
  return 0;
}
