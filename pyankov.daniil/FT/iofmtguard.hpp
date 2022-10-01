#ifndef IO_FMT_GUARD
#define IO_FMT_GUARD
#include <ios>
namespace pyankov
{
  class ioFmtGuard
  {
  public:
    ioFmtGuard(std::basic_ios< char >& s);
    ~ioFmtGuard();
  private:
    std::basic_ios< char >& s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

}
#endif
