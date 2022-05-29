#ifndef EXPR_ELEM_IO_HPP
#define EXPR_ELEM_IO_HPP

#include <iosfwd>
#include <memory>
#include <map>
#include <string>

#include "Function.hpp"

namespace semichev
{
  struct FunctionIO
  {
    std::shared_ptr< Function > func;
  };

  std::istream& operator>>(std::istream& in, FunctionIO& dest);
}

#endif
