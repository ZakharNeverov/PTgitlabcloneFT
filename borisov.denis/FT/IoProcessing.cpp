#include "IoProcessing.h"
#include <istream>
#include "DelimiterIO.h"
#include "IoFmtGuard.h"

std::istream& borisov::restoreIStream(std::istream& in)
{
  in.clear();
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  return in;
}

std::istream& borisov::getDict(std::istream& in, borisov::Dict& dict)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  IoFmtGuard guard(in);
  in >> std::noskipws;
  while (in)
  {
    std::string word;
    std::string translation;
    std::string countStr;
    in >> word >> DelimiterIO{' '} >> translation >> DelimiterIO{' '} >> countStr;
    size_t count = std::stoull(countStr);
    if (in)
    {
      borisov::add(dict, word, translation, count);
    }
    char ch = in.peek();
    if (ch == '\n')
    {
      break;
    }
    else
    {
      in >> DelimiterIO{' '};
    }
  }
  return in;
}

std::istream& borisov::getArgList(std::istream& in, borisov::ArgList& argList)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string s;
  IoFmtGuard guard(in);
  in >> std::noskipws;
  while (in)
  {
    in >> s;
    if (in)
    {
      argList.push_back(s);
    }
    char ch = in.peek();
    if (ch == '\n')
    {
      in >> DelimiterIO{'\n'};
      break;
    }
    else
    {
      in >> DelimiterIO{' '};
    }
  }
}
