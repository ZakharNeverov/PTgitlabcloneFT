#include "errorhandler.hpp"
#include <limits>
#include <iostream>

void krylyanok::streamClean(std::istream& in)
{
  in.clear();
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
}
