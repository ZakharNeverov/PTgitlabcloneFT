#ifndef IOFMTGUARD_HPP
#define IOFMTGUARD_HPP
#include <iosfwd>
#include <iomanip>
namespace yuldashev
{
  class iofmtguard
  {
  public:
    iofmtguard(std::basic_ios< char >& s);
    ~iofmtguard();
  private:
    std::basic_ios< char >& s_;
    char fill_;
    std::streamsize prec_;
    std::basic_ios< char >::fmtflags flags_;
  };
}
#endif
