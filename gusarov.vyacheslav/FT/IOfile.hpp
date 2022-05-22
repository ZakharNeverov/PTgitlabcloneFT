#ifndef IO_FILE_HPP
#define IO_FILE_HPP

#include <string>

namespace gusarov {
  struct nameFileIO
  {
    std::string& ref;
  };
  std::ifstream openFile(std::string name);
  std::string getSymbol(char ch);
  char getSymbol(std::string str);

  std::istream& operator>>(std::istream& in, nameFileIO&& dest);
}
#endif
