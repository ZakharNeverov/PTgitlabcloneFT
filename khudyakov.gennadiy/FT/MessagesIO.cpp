#include "MessagesIO.h"
#include <iostream>

namespace khudyakov
{
  std::ostream& invalidCommandMessage(std::ostream& stream)
  {
    stream << "<INVALID COMMAND>";
    return stream;
  }

  std::ostream& emptyMessage(std::ostream& stream)
  {
    stream << "<EMPTY>";
    return stream;
  }

  std::ostream& invalidArgumentMessage(std::ostream& stream)
  {
    stream << "<INVALID ARGUMENT>";
    return stream;
  }

  std::ostream& errorOpenFileMessage(std::ostream& stream)
  {
    stream << "<ERROR OPEN FILE>";
    return stream;
  }
}
