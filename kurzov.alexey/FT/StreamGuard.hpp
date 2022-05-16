#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP

#include <iostream>

namespace kurzov
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char >&);
    StreamGuard(const StreamGuard&) = delete;
    ~StreamGuard();
    StreamGuard& operator=(const StreamGuard&) = delete;

  private:
    std::basic_ios< char >& stream_;
    std::streamsize precision_;
    char fill_;
    std::basic_ios< char >::fmtflags flags_;
  };
}

#endif
