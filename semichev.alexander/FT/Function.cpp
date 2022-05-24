#include "Function.hpp"

std::istream& semichev::operator>>(std::istream& in, Function& func)
{
  return func.read(in);
}

std::ostream& semichev::operator<<(std::ostream& out, const std::pair< std::pair< int, int >, Function& >& func)
{
  return func.second.print(out, func.first);
}
