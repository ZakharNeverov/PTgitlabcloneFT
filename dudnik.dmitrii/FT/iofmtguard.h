#ifndef IO_FMT_GUARD_H
#define IO_FMT_GUARD_H
#include <iosfwd>
#include <iomanip>
namespace dudnik
{
  class iofmtguard
  {
  public:
    iofmtguard(std::basic_ios< char >&);
    ~iofmtguard();
  private:
    std::basic_ios< char >& s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}

#endif
