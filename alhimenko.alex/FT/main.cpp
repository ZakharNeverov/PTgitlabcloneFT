#include <iostream>
#include <fstream>

#include "user_interface.hpp"

int main()
{
  std::string IN_FILE = "input.txt";
  std::ifstream in(IN_FILE);
  alhimenko::User_interface ui;
  ui.start(std::cin, std::cout);

  return 0;
}
