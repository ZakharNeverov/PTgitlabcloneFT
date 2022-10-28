#ifndef DATA_PROCESSING_HPP
#define DATA_PROCESSING_HPP

#include <list>
#include <string>

namespace sviridov
{
  using CommandArgs = std::list< std::string >;

  CommandArgs getArguments(std::string data, char separator);
}

#endif
