#ifndef IOCTL_HPP
#define IOCTL_HPP

#include <string>

namespace abraamyan
{
  struct FlushWsIO
  {
    bool removeNl = false;
  };

  struct CommandIO
  {
    std::string &cmd;
  };

  struct ArgIO
  {
    std::string &arg;
    bool lastArg = false;
  };

  std::istream &operator>>(std::istream &in, FlushWsIO flushWs);
  std::istream &operator>>(std::istream &in, CommandIO command);
  std::istream &operator>>(std::istream &in, ArgIO arg);

  bool isLineEmpty(std::istream &in);

}

#endif
