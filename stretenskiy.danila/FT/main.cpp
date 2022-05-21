#include <iostream>
#include <fstream>
#include <map>
#include <functional>
#include "command-system.hpp"
#include "ioKeys.hpp"

int main(int argc, char* argv[])
{
  if (argc <= 1)
  {
    std::cerr << "Oh my God, I don't see the file\n";
    return 1;
  }
  std::ifstream in(argv[1]);
  if (!in.is_open())
  {
    std::cerr << "I can't open the file, I have paws\n";
    return 1;
  }
  using namespace stretenskiy;
  using namespace stretenskiy::function;

  nameDict name;
  vecDict hashT;
  readingDict(in, hashT, name);

  using namespace std::placeholders;
  std::map< std::string, std::function< void(std::ostream &) > > commands(
  {
    {"CREATION", std::bind(creationDict, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"ADD", std::bind(add, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"REMOVE", std::bind(removeWord, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"PRINT", std::bind(print, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"SEARCH", std::bind(search, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"CLEAR", std::bind(clearDict, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"UNION", std::bind(unionDict, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"INTERSECTION", std::bind(intersectDict, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))}
  });

  while (!std::cin.eof())
  {
    std::cin.clear();
    std::string command;
    std::cin >> command;
    auto iter = commands.find(command);
    if (iter != commands.end())
    {
      try
      {
        iter->second(std::cout);
      }
      catch (const std::exception &e)
      {
        std::cout << e.what() << '\n';
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }

  std::ofstream out(argv[1], std::ios::trunc);
  if (!out.is_open())
  {
    std::cerr << "I can't open the file, I have paws\n";
    return 1;
  }
  for (size_t i = 0; i < hashT.size(); ++i)
  {
    out << "Dictionary " << name[i] << '\n';
    out << hashT[i];
  }
  out.close();
  return 0;
}
