#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <stdexcept>
#include <functional>
#include <type_traits>
#include "matrix.hpp"
#include "map-commands.hpp"
#include "print-messages.hpp"
namespace pyankov
{
  template< typename T >
  int Matrix< T >::minValue_ = 0;
  template< typename T >
  int Matrix< T >::maxValue_ = 0;
  template< typename T >
  size_t Matrix< T >::precision_ = 0;
  template< typename T >
  void setStaticValues()
  {
    int minValue = 0;
    int maxValue = 0;
    size_t precision = 1;
    std::string type = "int";
    if (std::is_same< T, double >::value)
    {
      type = "double";
    }
    std::cout << "Input the min random value for " << type << " matrices: ";
    std::cin >> minValue;
    std::cout << "Input the max random value for " << type << " matrices: ";
    std::cin >> maxValue;
    if (std::is_same< T, double >::value)
    {
      std::cout << "Input precision for double matrix: ";
      std::cin >> precision; 
    }
    Matrix< T >::setStaticValues(minValue, maxValue, precision);
  }
}
int main()
{
  try
  {
    pyankov::setStaticValues< int >();
    pyankov::setStaticValues< double >();
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << "\n";
    return 1;
  }
  using int_chain_t = std::map< std::string, pyankov::Matrix< int > >;
  using double_chain_t = std::map< std::string, pyankov::Matrix< double > >;
  using chain_t = std::pair< int_chain_t, double_chain_t >;
  chain_t chain;
  std::map< std::string, std::function< void() > > commands(
    {
      {"create", std::bind(pyankov::create, std::ref(chain), std::ref(std::cin))},
      {"multiply", std::bind(pyankov::multiply, std::ref(chain), std::ref(std::cin))},
      {"add", std::bind(pyankov::add, std::ref(chain), std::ref(std::cin))},
      {"sub", std::bind(pyankov::sub, std::ref(chain), std::ref(std::cin))},
      {"concat", std::bind(pyankov::concat, std::ref(chain), std::ref(std::cin))},
      {"print", std::bind(pyankov::print, std::ref(chain), std::ref(std::cin), std::ref(std::cout))},
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
