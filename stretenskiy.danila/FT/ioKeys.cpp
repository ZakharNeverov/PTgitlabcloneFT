#include "ioKeys.hpp"
#include <iostream>

namespace stretenskiy
{
  IoFmtGuard::IoFmtGuard(std::basic_ios< char > &s):
    s_(s),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
  {}

  IoFmtGuard::~IoFmtGuard()
  {
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }

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
