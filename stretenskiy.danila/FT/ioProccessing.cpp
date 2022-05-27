#include "ioProccessing.hpp"

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

  bool checkContinueInputWord(std::istream &in)
  {
    stretenskiy::IoFmtGuard fmtGuard(in);
    in >> std::noskipws >> stretenskiy::DelimeterIO{' '};
    return static_cast< bool >(in);
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

  std::ostream &operator<<(std::ostream &out, const function::myDict &dict)
  {
    for (auto map_it = dict.begin(); map_it != dict.end(); ++map_it)
    {
      out << map_it->first << ":";
      for (const auto &i : map_it->second)
      {
        out << ' ' << i;
      }
      out << '\n';
    }
    return out;
  }

  void readingDict(std::istream &in, function::vecDict &vecDict, function::nameDict &nameDict)
  {
    std::string string;
    while (!in.eof() && in >> string)
    {
      if (string == "DICTIONARY")
      {
        std::string nameD;
        in >> nameD;
        nameDict.push_back(nameD);
        function::myDict temp;
        vecDict.push_back(temp);
      }
      else if (string[string.length() - 1] == ':')
      {
        string.pop_back();
        while (checkContinueInputWord(in))
        {
          std::string transl;
          in >> transl;
          vecDict[vecDict.size() - 1][string].insert(transl);
        }
        in.clear();
      }
    }
  }
}
