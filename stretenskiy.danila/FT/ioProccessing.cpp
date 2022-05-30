#include "ioProccessing.hpp"

#include <iostream>
#include <limits>

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

  bool checkContinueInputWord(std::istream &in)
  {
    stretenskiy::IoFmtGuard fmtGuard(in);
    in >> std::noskipws >> stretenskiy::DelimeterIO{' '};
    return static_cast< bool >(in);
  }

  void doCleanStream(std::istream &in)
  {
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

  std::istream &operator>>(std::istream &in, DelimeterIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
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

  std::istream &operator>>(std::istream &in, DelimeterNameIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    std::string str;
    in >> str;
    if (in && (str != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, WordIO &&word)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimeterIO{'"'}, word.ref, '"');
  }

  std::istream &operator>>(std::istream &in, NameDictIO &&nameD)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> DelimeterNameIO{"DICTIONARY"} >> WordIO{nameD.ref};
    return in;
  }

  std::istream &operator>>(std::istream &in, TranslateIO &&transl)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    do
    {
      std::string temp;
      in >> WordIO{temp};
      transl.ref.insert(temp);
    }
    while(in >> DelimeterIO{','});
    in.clear();
    return in;
  }
}
