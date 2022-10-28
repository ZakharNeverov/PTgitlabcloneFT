#ifndef IOFMTGUARD_HPP
#define IOFMTGUARD_HPP

#include <ios>

namespace varnavskii
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
}

#endif
