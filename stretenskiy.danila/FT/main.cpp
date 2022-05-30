#include <iostream>
#include <fstream>
#include <map>
#include <functional>
#include <iterator>
#include <stdexcept>
#include "command-system.hpp"
#include "ioProccessing.hpp"
#include "warningMessages.hpp"
#include "dictionary.hpp"

int main(int argc, char* argv[])
{
  if (argc <= 1)
  {
    stretenskiy::informBadArgs(std::cerr);
    return 1;
  }
  std::setlocale(LC_ALL, "rus");
  std::ifstream in(argv[1]);
  if (!in.is_open())
  {
    stretenskiy::informNotOpenFile(std::cerr);
    return 1;
  }
  using namespace stretenskiy;
  using namespace stretenskiy::function;
  using is_it = std::istream_iterator< Dictionary >;
  vecDict hashT;
  while (!in.eof())
  {
    std::copy(is_it(in), is_it(), std::back_inserter(hashT));
    if (!in)
    {
      doCleanStream(in);
    }
  }
  in.close();

  std::copy(hashT.begin(), hashT.end(), std::ostream_iterator< Dictionary >(std::cout, "\n"));

  using namespace std::placeholders;
  std::map< std::string, std::function< void(std::ostream &) > > commands(
  {
    {"CREATION", std::bind(creationDict, _1, std::ref(hashT), std::ref(std::cin))},
    {"ADD", std::bind(add, _1, std::ref(hashT), std::ref(std::cin))},
    {"REMOVE", std::bind(removeWord, _1, std::ref(hashT), std::ref(std::cin))},
    {"PRINT", std::bind(print, _1, std::ref(hashT), std::ref(std::cin))},
    {"SEARCH", std::bind(search, _1, std::ref(hashT), std::ref(std::cin))},
    {"CLEAR", std::bind(clearDict, _1, std::ref(hashT), std::ref(std::cin))},
    {"UNION", std::bind(unionDict, _1, std::ref(hashT), std::ref(std::cin))},
    {"INTERSECTION", std::bind(intersectDict, _1, std::ref(hashT), std::ref(std::cin))}
  });

  while (!std::cin.eof())
  {
    std::cin.clear();
    std::string commandName;
    std::cin >> commandName;
    if (!commandName.empty())
    {
      try
      {
        commands.at(commandName)(std::cout);
      }
      catch (const std::exception &e)
      {
        stretenskiy::informNotFindCommand(std::cout);
        doCleanStream(std::cin);
      }
    }
  }

  std::ofstream out(argv[1], std::ios::trunc);
  std::copy(hashT.begin(), hashT.end(), std::ostream_iterator< Dictionary >(out, "\n"));
  out.close();
  return 0;
}
