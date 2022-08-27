#ifndef STREAMGUARD_HPP
#define STREAMGUARD_HPP
#include <iosfwd>
#include <iomanip>

namespace khudyakov
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char >& stream);
    ~StreamGuard();
  private:
    std::basic_ios< char >& stream_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags flags_;
  };
}

#endif
