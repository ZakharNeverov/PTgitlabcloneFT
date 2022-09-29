#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <functional>

#include "commands.hpp"

namespace alhimenko
{

  class User_interface
  {
  public:
    User_interface();
    void start(std::istream&, std::ostream& out);

    ~User_interface() = default;

  private:
    using command = std::function<decltype(alhimenko::createGraph)>;

    std::map<std::string, command> commands_;
  };
}
#endif
