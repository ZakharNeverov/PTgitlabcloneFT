#include "inputProcess.hpp"

std::string surby::getNextArg(std::string& line, size_t k)
{
  std::string str;
  size_t i = 0;
  for (i = 0; i < line.size() && line[i] != ' ' && line[i] != '\n'; i++)
  {
    str += line[i];
  }
  if (line[i] == ' ' && str.size() == 0)
  {
    str = " ";
    i++;
  }
  if (line[i] == '\0' || i == line.size())
  {
    line = '\0';
  }
  else
  {
    line = line.substr(i + k, line.size());
  }
  return str;
}
