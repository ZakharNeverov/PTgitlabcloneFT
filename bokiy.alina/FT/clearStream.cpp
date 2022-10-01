#include "clearStream.hpp"
#include <iostream>
#include <limits>

std::istream& bokiy::clearIStream(std::istream& in)
{
  in.clear();
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  return in;
}
