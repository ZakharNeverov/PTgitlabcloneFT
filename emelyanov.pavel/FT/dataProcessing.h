#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <list>
#include <string>

namespace emelyanov {
  using CommandArgs = std::list< std::string >;

  CommandArgs getArguments(std::string data, char separator);
}

#endif
