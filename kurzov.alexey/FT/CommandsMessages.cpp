#include "CommandsMessages.hpp"
#include <iostream>

void kurzov::outError(std::ostream& stream)
{
  stream << "<INVALID COMMAND>\n";
}

void kurzov::outEmpty(std::ostream& stream)
{
  stream << "<EMPTY>\n";
}

void kurzov::outTrue(std::ostream& stream)
{
  stream << "<TRUE>\n";
}

void kurzov::outFalse(std::ostream& stream)
{
  stream << "<FALSE>\n";
}

void kurzov::outNone(std::ostream& stream)
{
  stream << "<NONE>\n";
}

