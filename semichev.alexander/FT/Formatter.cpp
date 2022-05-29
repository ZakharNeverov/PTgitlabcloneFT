#include "formatter.hpp"

semichev::Formatter::Formatter(std::ios_base& ios):
  ios_(ios),
  precision_(ios.precision()),
  flags_(ios_.flags())
{}

semichev::Formatter::~Formatter()
{
  ios_.precision(precision_);
  ios_.flags(flags_);
}
