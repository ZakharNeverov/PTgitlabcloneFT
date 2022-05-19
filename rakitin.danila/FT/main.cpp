#include <iomanip>
#include <iostream>
#include <string>

#include "commands.h"
#include "dictionary_usings.h"
#include "dictionary_utils.h"
#include "iofmtguard.h"

int main()
{
  rakitin::dictionaries data;
  rakitin::iofmtguard guard(std::cout);
  std::cout << std::fixed << std::setprecision(1);
  while (!std::cin.eof()) {
    try {
      std::cout << "Write command:\n";
      std::string command;
      std::cin >> command;
      if (command == "ADD") {
        std::string path;
        std::string dataname;
        std::cin >> path >> dataname;
        rakitin::add(data, dataname, path);
        std::cout << "Dictionary " << dataname << " has been added\n";
      }
      else if (command == "FINDWORD") {
        std::string word;
        std::string dataname;
        std::cin >> word >> dataname;
        rakitin::ref_finder finder = rakitin::findWord(data, dataname, word);
        if (finder.second) {
          rakitin::printWord(finder.first, std::cout);
          std::cout << "\n";
        }
        else {
          std::cout << "The word does not occur in the text\n";
        }
      }
      else if (command == "PRINTALL") {
        std::string dataname;
        std::cin >> dataname;
        rakitin::printAll(data, dataname, std::cout);
        std::cout << "\n";
      }
      else if (command == "MERGE") {
        std::string dataname1, dataname2;
        std::cin >> dataname1 >> dataname2;
        rakitin::merge(data, dataname1, dataname2);
        std::cout << "Dictionaries successfully merged\n";
      }
      else if (command == "CLEAR") {
        std::string dataname1;
        std::cin >> dataname1;
        rakitin::clear(data, dataname1);
        std::cout << "Dictionary was cleared\n";
      }
      else if (command == "INTERSECTION") {
        std::string dataname1, dataname2;
        std::cin >> dataname1 >> dataname2;
        rakitin::intersection(data, dataname1, dataname2);
        std::cout << "Intersection dictionary was created with name " << dataname1 << "&" << dataname2
          << "_intersection\n";
      }
      else if (command == "SIMILARITY") {
        std::string dataname1, dataname2;
        std::cin >> dataname1 >> dataname2;
        double sim = rakitin::similarity(data, dataname1, dataname2) * 100;
        std::cout << "Similarity: " << sim << "%\n";
      }
      else if (command == "DELETE") {
        std::string dataname1;
        std::cin >> dataname1;
        rakitin::deleteDictionary(data, dataname1);
        std::cout << "Dictionary was deleted\n";
      }
      else if (command == "RENAME") {
        std::string dataname, newDataname;
        std::cin >> dataname >> newDataname;
        rakitin::rename(data, dataname, newDataname);
        std::cout << "Dictionary was renamed\n";
      }
      else if (command == "PRINT_ALL_DATANAMES") {
        rakitin::printAllDatanames(data, std::cout);
        std::cout << "\n";
      }
      else {
        std::cout << "Unknown command\n";
      }
    }
    catch (const std::exception& e) {
      std::cout << e.what() << "\n";
      return 1;
    }
  }
  return 0;
}
