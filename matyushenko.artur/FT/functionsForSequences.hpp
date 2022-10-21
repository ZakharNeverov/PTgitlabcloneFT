#ifndef FT_FUNCTIONSFORSEQUENCES_HPP
#define FT_FUNCTIONSFORSEQUENCES_HPP
#include <map>
#include <string>
namespace matyushenko
{
  using dictionary = std::map< std::string, std::string >;
  using str = std::string;
  std::string findLongestSub(str &, str &);
}
#endif
