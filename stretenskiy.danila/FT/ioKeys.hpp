#ifndef IOKEYS_HPP
#define IOKEYS_HPP
#include <ios>

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

  std::istream &operator>>(std::istream &, DelimeterIO &&);
}

#endif
