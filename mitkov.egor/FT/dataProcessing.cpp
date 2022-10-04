#include "dataProcessing.h"
#include <cstddef>

namespace {
  using CmdArgs = mitkov::CommandArgs;

  std::string getWord(std::string& data, char sep)
  {
    size_t separatorIndex = data.find_first_of(sep, 0);
    std::string result = data.substr(0, separatorIndex);
    separatorIndex = (separatorIndex == data.npos) ? separatorIndex : separatorIndex + 1;
    data.erase(0, separatorIndex);
    return result;
  }
}

CmdArgs mitkov::getArguments(std::string data, char separator)
{
  CmdArgs args;
  size_t size = data.size();
  for (size_t i = 0; data.size() != 0 && i < size;) {
    std::string result = getWord(data, separator);
    args.push_back(result);
  }
  return args;
}
