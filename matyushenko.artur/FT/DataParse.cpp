#include "DataParse.hpp"
namespace matyushenko
{
  std::string getPartOfString(std::string &data)
  {
    size_t pos = data.find_first_of(' ', 0);
    std::string part = data.substr(0, pos);
    if (pos != data.npos)
    {
      pos++;
    }
    data.erase(0, pos);
    return part;
  }
}
