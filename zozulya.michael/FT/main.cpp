#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <limits>
#include "huffman.h"
#include "commands.h"
#include "print-messages.h"
int main()
{
  setlocale(LC_ALL, "Russian");
  std::map< std::string, std::function< void() > > shell = {
  {"readFile", std::bind(&zozulya::readFileCMD, std::ref(std::cin))},
  {"fillFreq", std::bind(&zozulya::fillFrequencyCMD, std::ref(std::cin))},
  {"printCodeDiff", std::bind(&zozulya::printCodeDifferenceCMD, std::ref(std::cin))},
  {"toCode", std::bind(&zozulya::toCode, std::ref(std::cin))},
  {"compressionFiles", std::bind(&zozulya::compressionFiles, std::ref(std::cin))},
  {"compressionAllFiles", std::bind(&zozulya::compressionAllFiles, std::ref(std::cin))},
  {"delta", std::bind(&zozulya::delta, std::ref(std::cin))},
  {"isFileEncrypted", std::bind(&zozulya::isFileEncrypted, std::ref(std::cin))}};
  std::string cmd;
  while (!std::cin.eof())
  {
    std::cin.clear();
    std::string mainCommandName = "";
    std::cin >> mainCommandName;
    if (mainCommandName.empty())
    {
      continue;
    }
    try
    {
      shell[mainCommandName]();
    }
    catch (const std::exception& e)
    {
      std::cerr << "INVALID COMMAND\n\n";
      std::cin.clear();
    }
  }
  return 0;
}
//README
/*
Пример команд:
1)readFile file1
2)fillFreq file2
3)printCodeDiff file3
4)toCode inputFile outputFile
5)compressionFiles nameFile1 nameFile2
6)compressionAllFiles file1 file2
7)delta file
8)isFileEncrypted FileToCode
*/
