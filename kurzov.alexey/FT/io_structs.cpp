#include "io_structs.hpp"
#include <iostream>

std::istream& kurzov::operator>>(std::istream& in, DelimeterIO&& dest)
{
  if (!std::istream::sentry(in))
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
