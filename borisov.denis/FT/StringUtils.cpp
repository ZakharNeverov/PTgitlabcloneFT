#include "StringUtils.h"
#include <string>

void borisov::utils::split(const std::string& s, std::list< std::string >& list, char separator)
{
  size_t start = 0;
  size_t end = start;
  std::string substr;
  while ((end = s.find(separator, start)) != std::string::npos)
  {
    substr = s.substr(start, end - start);
    list.push_back(substr);
    start = end + 1;
  }
  end = s.size();
  substr = s.substr(start, end - start);
  list.push_back(substr);
}
