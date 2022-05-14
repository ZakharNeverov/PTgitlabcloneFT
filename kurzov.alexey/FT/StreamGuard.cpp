#include "StreamGuard.hpp"

kurzov::StreamGuard::StreamGuard(std::basic_ios< char >& stream):
  stream_(stream),
  precision_(stream.precision()),
  fill_(stream.fill()),
  flags_(stream.flags())
{}

kurzov::StreamGuard::~StreamGuard()
{
  stream_.precision(precision_);
  stream_.fill(fill_);
  stream_.flags(flags_);
}
