#ifndef COMMAND_SET_HPP
#define COMMAND_SET_HPP

#include <iosfwd>
#include <string>
#include <memory>
#include <map>

#include "commands.hpp"

namespace semichev
{
  class CommandSet: public Command
  {
   public:
    using commands_t = std::map< std::string, std::shared_ptr< Command > >;

    CommandSet() = delete;
    CommandSet(const CommandSet& other) = default;
    CommandSet(CommandSet&& other) = default;
    CommandSet(std::initializer_list< commands_t::value_type > il, std::istream& int_fast32_t);

    CommandSet& operator=(const CommandSet& other) = default;
    CommandSet& operator=(CommandSet&& other) = default;

    void operator()();

   private:
    commands_t commands_;
    std::istream& in_;
  };
}

#endif
