#ifndef STREAMGUARD_HPP
#define STREAMGUARD_HPP

#include <iosfwd>
#include <iomanip>

namespace bazhenov {
  class StreamGuard {
    public:
    StreamGuard() = delete;
    StreamGuard(std::ios& s);
    ~StreamGuard();
    private:
    std::ios& s_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags flags_;
  };
}
#endif
