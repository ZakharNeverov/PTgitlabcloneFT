#include "print-messages.hpp"
#include <iostream>
std::ostream& pyankov::printInvalidCommand(std::ostream& out)
{
  out << "<INVALID COMMAND>\n";
  return out;
}
std::ostream& pyankov::printTrue(std::ostream& out)
{
  out << "<TRUE>\n";
  return out;
}
std::ostream& pyankov::printFalse(std::ostream& out)
{
  out << "<FALSE>\n";
  return out;
}
