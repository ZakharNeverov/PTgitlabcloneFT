#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <stdexcept>
#include <functional>
#include "matrix.hpp"
#include "map-commands.hpp"
#include "print-messages.hpp"
int main()
{
  int minValue = 0;
  int maxValue = 0;
  size_t precision = 0;
  std::cin >> minValue >> maxValue >> precision;
  if (!std::cin)
  {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return 1;
  }
  using int_chain_t = std::map< std::string, pyankov::Matrix< int > >;
  using double_chain_t = std::map< std::string, pyankov::Matrix< double > >;
  using chain_t = std::pair< int_chain_t, double_chain_t >;
  chain_t chain;
  std::map< std::string, std::function< void() > > commands(
    {
      {"create", std::bind(pyankov::create, std::ref(chain), std::ref(std::cin), minValue, maxValue, precision)},
      {"multiply", std::bind(pyankov::multiply, std::ref(chain), std::ref(std::cin))},
      {"add", std::bind(pyankov::add, std::ref(chain), std::ref(std::cin))},
      {"sub", std::bind(pyankov::sub, std::ref(chain), std::ref(std::cin))},
      {"concat", std::bind(pyankov::concat, std::ref(chain), std::ref(std::cin))},
      {"print", std::bind(pyankov::print, std::ref(chain), std::ref(std::cin), std::ref(std::cout), precision)},
      {"equals", std::bind(pyankov::equals, std::ref(chain), std::ref(std::cin), std::ref(std::cout))}
    }
  );
  while (!std::cin.eof())
  {
    std::cin.clear();
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
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
