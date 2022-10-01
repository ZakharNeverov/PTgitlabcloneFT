#include "Utilites.hpp"
#include <fstream>
#include <stdexcept>

std::string roletskaya::getElem(std::string& string)
{
  size_t spaceIndex = string.find_first_of(' ', 0);
  std::string elem = string.substr(0, spaceIndex);
  if (spaceIndex != string.npos)
  {
    spaceIndex++;
  }
  string.erase(0, spaceIndex);
  return elem;
}

std::string roletskaya::getNameFromFile(std::string& string)
{
  return string.substr(0, string.find_first_of('.'));
}

bool roletskaya::checkResults(std::string& outFileName, std::string& resultsFileName)
{
  std::ifstream in;
  std::ifstream checkIn;
  in.open(outFileName);
  checkIn.open(resultsFileName);
  if (!checkIn)
  {
    throw std::invalid_argument("Couldn't open the result file.\n");
  }
  std::string line1 = "";
  std::string line2 = "";
  while (!in.eof() || !checkIn.eof())
  {
    std::getline(in, line1);
    std::getline(checkIn, line2);
    if (line1 != line2)
    {
      in.close();
      checkIn.close();
      return false;
    }
  }
  in.close();
  checkIn.close();
  if ((in.eof() && !checkIn.eof()) || (!in.eof() && checkIn.eof()))
  {
    return false;
  }
  return true;
}
