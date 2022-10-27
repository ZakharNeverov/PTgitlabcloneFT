#include <iostream>
#include <functional>
#include "consoleCommands.hpp"
#include "iofunctions.hpp"

namespace neverov
{
  std::string cmd = "";
  texts_t texts{};
  std::map< std::string, std::function< void() > > commands
   {
     { "createText", std::bind(createText, std::ref(cmd), std::ref(texts), std::ref(std::cin)) },
     { "getText", std::bind(getText, std::ref(cmd), std::ref(texts), std::ref(std::cout)) },
     { "encode", std::bind(encode, std::ref(cmd), std::ref(texts)) },
     { "decode", std::bind(decode, std::ref(cmd), std::ref(texts)) },
     { "getDict", std::bind(getDict, std::ref(cmd), std::ref(texts)) },
     { "getCompDiff", std::bind(getCompDiff, std::ref(cmd), std::ref(texts), std::ref(std::cout)) },
     { "mergeText", std::bind(mergeText, std::ref(cmd), std::ref(texts)) },
   };
}

int main()
{
  while (!std::cin.eof())
  {
    neverov::cmd = "";
    std::getline(std::cin, neverov::cmd);
    if (!neverov::cmd.empty())
    {
      std::string command = "";
      if (neverov::getNextWord(neverov::cmd, command))
      {
        try
        {
          neverov::commands.at(command)();
        }
        catch (const std::out_of_range&)
        {
          std::cout << "<No such command>\n";
        }
        catch (const std::invalid_argument& e)
        {
          std::cout << e.what() << '\n';
        }
      }
    }
  }
}
