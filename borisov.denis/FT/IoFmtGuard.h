#ifndef IO_FMT_GUARD_H
#define IO_FMT_GUARD_H
#include <iosfwd>
#include <iomanip>

namespace borisov
{
  class IoFmtGuard
  {
  public:
    IoFmtGuard(std::basic_ios< char >& s);
    ~IoFmtGuard();
  private:
    std::basic_ios< char >& s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}
#endif
