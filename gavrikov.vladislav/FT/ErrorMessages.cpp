#include "ErrorMessages.hpp"
#include <ostream>

std::ostream& gavrikov::errorNameMessage(std::ostream& out)
{
  return out << "<There is no dictionary by that name!>";
}
std::ostream& gavrikov::emptyCollectionMessage(std::ostream& out)
{
  return out << "<There are no words in this dictionary!>";
}
std::ostream& gavrikov::nameRedefinitionMessage(std::ostream& out)
{
  return out << "<A similar dictionary already exists!>";
}
std::ostream& gavrikov::errorOpenFile(std::ostream& out)
{
  return out << "<The file was not opened!>";
}
