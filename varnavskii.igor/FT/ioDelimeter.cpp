#include "ioDelimeter.hpp"

#include <iostream>

namespace varnavskii
{
  std::istream &operator>>(std::istream &stream, DelimeterIO &&dest)
  {
    std::istream::sentry sentry(stream);
    if (!sentry)
    {
      return stream;
    }
    char c = '0';
    stream >> c;
    if (stream && (c != dest.exp))
    {
      stream.setstate(std::ios::failbit);
    }
    return stream;
  }
}
