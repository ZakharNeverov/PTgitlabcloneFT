#include <iostream>
#include <fstream>
#include <vector>
#include "Dictionary.hpp"
#include "Utilities.hpp"

int main()
{
  shkroba::Dictionary d1;
  shkroba::Dictionary d2;
  shkroba::Dictionary d3;
  shkroba::Dictionary d4;
  shkroba::Dictionary d5;

  try
  {
    shkroba::createTestDictionaries(d1, d2,d3, d4,d5);
    shkroba::testCommandSystem(d1, d2, d3, d4, d5);
//    shkroba::help(std::cout);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what();
  }
  return 0;
}
