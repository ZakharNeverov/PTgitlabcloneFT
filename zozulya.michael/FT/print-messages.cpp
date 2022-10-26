#include "print-messages.h"
#include <iostream>

std::ostream& zozulya::printInvalidCommand(std::ostream& out)
{
  out << "<INVALID COMMAND>\n";
  return out;
}
