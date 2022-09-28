#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <list>
#include <string>

namespace mitkov {
  using CommandArgs = std::list< std::string >;

  CommandArgs getArguments(std::string data, char separator);
}

#endif
