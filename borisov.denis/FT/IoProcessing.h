#ifndef IO_PROCESSING_H
#define IO_PROCESSING_H
#include <iosfwd>
#include "CommandWrappers.h"

namespace borisov
{
  std::istream& restoreIStream(std::istream& in);
  std::istream& getDict(std::istream& in, borisov::Dict& dict);
  std::ostream& printDict(std::ostream& out, const borisov::Dict& dict);
  std::istream& getArgList(std::istream& in, borisov::ArgList& argList);
}
#endif
