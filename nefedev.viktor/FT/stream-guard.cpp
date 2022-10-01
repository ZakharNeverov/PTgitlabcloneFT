#include "stream-guard.hpp"

nefedev::StreamGuard::StreamGuard(std::basic_ios< char >& s):
  s_(s),
  precision_(s.precision()),
  fmt_(s.flags())
{}

nefedev::StreamGuard::~StreamGuard()
{
  s_.precision(precision_);
  s_.flags(fmt_);
}
