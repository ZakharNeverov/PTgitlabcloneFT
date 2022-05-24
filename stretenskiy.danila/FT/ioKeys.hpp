#ifndef IOKEYS_HPP
#define IOKEYS_HPP

#include <ios>
#include "command-system.hpp"

namespace stretenskiy
{
  class IoFmtGuard
  {
  public:
    IoFmtGuard(std::basic_ios< char > &s);
    ~IoFmtGuard();
  private:
    std::basic_ios< char > &s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  struct DelimeterIO
  {
    char exp;
  };

  bool checkContinueInputWord(std::istream &);
  std::istream &operator>>(std::istream &, DelimeterIO &&);
  std::ostream &operator<<(std::ostream &, const function::myDict &);
  void readingDict(std::istream &, function::vecDict &, function::nameDict &);
}

#endif
