#include <iostream>
#include <algorithm>
#include <iterator>
#include <functional>
#include <string>
#include <map>
#include "commands.hpp"
#include "matrixChain.hpp"
#include "operations.hpp"
#include "print.hpp"
namespace yuld = yuldashev;
int main()
{
  yuld::MatrixChain matrixChain;
  std::map< std::string, std::function< bool() > > commands(
    {
      {"create", std::bind(yuld::doCreate, std::ref(std::cin), std::ref(matrixChain))},
      {"open", std::bind(yuld::doImport, std::ref(std::cin), std::ref(matrixChain))},
      {"multiply", std::bind(yuld::doMultiply, std::ref(std::cin), std::ref(matrixChain))},
      {"print", std::bind(yuld::doPrint, std::ref(std::cin), std::ref(std::cout), std::cref(matrixChain), false)},
      {"delete", std::bind(yuld::doDelete, std::ref(std::cin), std::ref(matrixChain))},
      {"save", std::bind(yuld::doSave, std::ref(std::cin), std::ref(matrixChain))},
      {"replaceat", std::bind(yuld::doReplaceAt, std::ref(std::cin), std::ref(matrixChain))},
      {"sum", std::bind(yuld::doSum, std::ref(std::cin), std::ref(matrixChain))},
      {"multiplybyelement", std::bind(yuld::doMultiplyByElement, std::ref(std::cin), std::ref(matrixChain))}
    });
  while (!std::cin.eof())
  {
    std::cin.clear();
    std::string cmdName = "";
    std::cin >> cmdName;
    if (cmdName.empty())
    {
      continue;
    }
    auto command = commands.find(cmdName);
    try
    {
      if (!(command != commands.end() && command->second()))
      {
        yuld::printInvalid(std::cout);
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
    catch (...)
    {
      yuld::printInvalid(std::cout);
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
