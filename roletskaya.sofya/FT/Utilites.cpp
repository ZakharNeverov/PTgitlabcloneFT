#include "Utilites.hpp"

std::string roletskaya::getElem(std::string& string)
{
  size_t spaceIndex = string.find_first_of(' ', 0);
  std::string elem = string.substr(0, spaceIndex);
  if (spaceIndex != string.npos)
  {
    spaceIndex++;
  }
  string.erase(0, spaceIndex);
  return elem;
}
