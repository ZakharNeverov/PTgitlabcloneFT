#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <utility>

namespace alhimenko
{
  std::istream& skipblank(std::istream&);
  void skipcommand(std::istream&);
  class SkipWsFlagSaver
  {

  public:
    explicit SkipWsFlagSaver(std::istream& in);
    ~SkipWsFlagSaver();
  private:
    std::istream* in_;
    bool worked_ = false;
  };

  template < class Function >
  class Parser
  {

  public:
    explicit Parser(const std::map< std::string, Function >& commands) :
      commands_(commands) {}

    template< typename... Args >
    void start(std::istream& in, std::ostream& out, Args&&... args)
    {
      std::string cmd;
      while (in >> cmd)
      {
        if (in.bad())
        {
          throw std::runtime_error("Stream was broken");
        }
        if (in.fail())
        {
          throw std::invalid_argument("\n<INCORRECT COMMAND>\n");
        }

        if (commands_.find(cmd) != commands_.end())
        {
          commands_.at(cmd)(std::forward< Args >(args)...);
        }
        else std::cerr << "\n<INCORRECT COMMAND>\n";
      }
    }

  private:
    std::map< std::string, Function > commands_;

  };
}
#endif
