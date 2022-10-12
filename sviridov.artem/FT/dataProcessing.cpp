#include "dataProcessing.hpp"

namespace
{
  using cmdArgs = sviridov::CommandArgs;

  std::string getWord(std::string& data, char separator)
  {
    size_t separatorIndex = data.find_first_of(separator, 0);
    std::string result = data.substr(0, separatorIndex);
    if (separatorIndex != data.npos)
    {
      separatorIndex++;
    }
    data.erase(0, separatorIndex);
    return result;
  }
}

cmdArgs sviridov::getArguments(std::string data, char separator)
{
  cmdArgs args;
  size_t size = data.size();
  for (size_t i = 0; data.size() != 0 && i < size; i++)
  {
    if (data[i] != ' ')
    {
      std::string argument = getWord(data, separator);
      args.push_back(argument);
    }
  }
  return args;
}
