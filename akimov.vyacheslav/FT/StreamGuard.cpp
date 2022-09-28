#include "StreamGuard.hpp"
akimov::StreamGuard::StreamGuard(std::basic_ios< char >& stream) :
  stream_(stream),
  precision_(stream.precision()),
  flags_(stream.flags())
{}
akimov::StreamGuard::~StreamGuard()
{
  stream_.precision(precision_);
  stream_.flags(flags_);
}
