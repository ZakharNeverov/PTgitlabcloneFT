#include "streamGuard.hpp"

bazhenov::StreamGuard::StreamGuard(std::ios& s):
  s_(s),
  precision_(s.precision()),
  flags_(s.flags())
{}

bazhenov::StreamGuard::~StreamGuard()
{
  s_.precision(precision_);
  s_.flags(flags_);
}
