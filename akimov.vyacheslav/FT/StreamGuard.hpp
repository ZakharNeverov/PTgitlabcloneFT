#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP
#include <ios>
namespace akimov
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char >& stream);
    StreamGuard(const StreamGuard& stream) = delete;
    ~StreamGuard();
    StreamGuard& operator=(const StreamGuard& s) = delete;
  private:
    std::basic_ios< char >& stream_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags flags_;
  };
}
#endif
