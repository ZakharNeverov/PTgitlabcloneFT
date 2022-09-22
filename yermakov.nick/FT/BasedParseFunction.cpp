#include "BasedParseFunction.hpp"

std::string yermakov::getAndEraseWord(std::string& description)
{
  if (description.find_first_of(" ") == std::string::npos)
  {
    std::string returnValue = description;
    description = "";
    return returnValue;
  }
  std::string returnValue = description.substr(0, description.find_first_of(" "));
  description = description.substr(description.find_first_of(" ") + 1);
  return returnValue;
}
