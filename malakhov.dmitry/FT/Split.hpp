#ifndef SPLIT_HPP
#define SPLIT_HPP

#include <string>

#include "Commands.hpp"

namespace malakhov
{
  void split(const std::string& input, std::string& commandName, Commands::ArgsListT& args);
}

#endif
