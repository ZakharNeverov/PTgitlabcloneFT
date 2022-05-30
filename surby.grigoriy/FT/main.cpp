#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include <functional>
#include "Dictionary.hpp"
#include "Commands.hpp"

int main()
{
  std::locale::global(std::locale(".utf8"));
  std::string line;
  std::string def = "default";
  std::unordered_map < std::string, std::string > texts;
  std::map < std::string, std::function< void() > > commands;

  commands["addText"] = std::bind(surby::addTextCommand, std::ref(texts), std::ref(line), std::ref(std::cin));
  commands["getDictionary"] = std::bind(surby::getDictionaryCommand, std::ref(texts), std::ref(line), std::ref(std::cout));
  commands["getText"] = std::bind(surby::getTextCommand, std::ref(texts), std::ref(line), std::ref(std::cout));
  commands["encode"] = std::bind(surby::encodeCommand, std::ref(texts), std::ref(line));
  commands["decode"] = std::bind(surby::decodeCommand, std::ref(texts), std::ref(line));
  commands["encodeKey"] = std::bind(surby::encodeKeyCommand, std::ref(texts), std::ref(line));
  commands["checkDifference"] = std::bind(surby::checkDifferenceCommand, std::ref(texts), std::ref(line), std::ref(std::cout));

  while (std::getline(std::cin, line))
  {
    try
    {
      if (!std::cin.eof() && !line.empty())
      {
        std::string operation = surby::getNextArg(line, 1);
        commands.at(operation)();
        if (line[0] != '\0')
        {
          throw std::overflow_error("wrong input");
        }
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what();
      line = "\0";
    }
  }
  return 0;
}

/*addText name C:\S1in.txt
getDictionary name C:\S1out.txt
getText name C:\S1out.txt
encode C:\S1in.txt C:\S1out.txt name
decode name C:\S1out.txt name2
encodeKey name default name2
checkDifference name default*/
