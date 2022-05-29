#include "iofmtguard.hpp"

gusarov::iofmtguard::iofmtguard(std::basic_ios< char >& stream):
  stream_(stream),
  fill_(stream.fill()),
  precision_(stream.precision()),
  fmt_(stream.flags())
{}

gusarov::iofmtguard::~iofmtguard()
{
  stream_.fill(fill_);
  stream_.precision(precision_);
  stream_.flags(fmt_);
}
