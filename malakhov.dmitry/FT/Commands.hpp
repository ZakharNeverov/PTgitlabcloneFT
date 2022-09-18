#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>

#include "Dictionary.hpp"
#include "HaffmanStringUnion.hpp"
#include "ForwardList.hpp"

namespace malakhov
{
  class Commands
  {
  public:
    using StrT = std::string;
    using StrsDictT = malakhov::Dictionary< StrT, HaffmanStringUnion >;
    using ArgsListT = malakhov::ForwardList< StrT >;

    Commands(StrsDictT& strs, std::ostream& out);
    void call(const StrT& commandName, const ArgsListT& args) const noexcept;

  private:
    using CommandT = void (*)(StrsDictT&, const ArgsListT&);
    using ConstCommandT = void (*)(const StrsDictT&, const ArgsListT&, std::ostream&);

    using CommandsDictT = Dictionary< StrT, CommandT >;
    using ConstCommandsDictT = Dictionary< StrT, ConstCommandT >;

    StrsDictT& strs_;
    const CommandsDictT commands_;
    const ConstCommandsDictT constCommands_;
    std::ostream& out_;
  };
}

#endif
