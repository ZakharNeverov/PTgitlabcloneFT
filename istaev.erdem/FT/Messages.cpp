#include "Messages.h"
#include <iostream>

std::ostream& istaev::printInvalidCommand(std::ostream& out)
{
  return out << "Invalid command";
}

std::ostream& istaev::graphIsEmpty(std::ostream& out)
{
  return out << "Graph is empty!";
}

std::ostream& istaev::invalidArguments(std::ostream& out)
{
  return out << "Invalid arguments!";
}

std::ostream& istaev::edgeIsPresent(std::ostream& out)
{
  return out << "Edge is present!";
}

std::ostream& istaev::edgeIsAbsent(std::ostream& out)
{
  return out << "Edge is absent!";
}

std::ostream& istaev::vertexIsAbsent(std::ostream& out)
{
  return out << "Vertex is absent!";
}
