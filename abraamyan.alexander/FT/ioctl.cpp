#include "ioctl.hpp"
#include <iostream>

namespace abraamyan
{
  std::istream &operator>>(std::istream &in, FlushWsIO flushWs)
  {
    if (in.eof())
    {
      return in;
    }
    std::istream::sentry sen(in, true);
    if (!sen)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    if (flushWs.removeNl)
    {
      while (!in.eof() && std::isspace(in.peek()))
      {
        in.ignore(1);
      }
    }
    else
    {
      while (!in.eof() && std::isspace(in.peek()) && in.peek() != '\n')
      {
        in.ignore(1);
      }
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, CommandIO command)
  {
    std::istream::sentry sen(in);
    if (!sen)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::string cmd;
    in >> cmd;
    if (in)
    {
      command.cmd = std::move(cmd);
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, ArgIO arg)
  {
    std::istream::sentry sen(in, true);
    if (!sen)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    if (isLineEmpty(in))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::string tmpArg;
    in >> tmpArg;
    if (!in || (arg.lastArg && !isLineEmpty(in)))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    arg.arg = std::move(tmpArg);
    return in;
  }

  bool isLineEmpty(std::istream &in)
  {
    in >> FlushWsIO{  };
    if (in.eof() || in.peek() == '\n')
    {
      return true;
    }
    return false;
  }
}
