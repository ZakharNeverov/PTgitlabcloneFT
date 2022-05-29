#include <iostream>
#include <algorithm>
#include <functional>
#include "dictionaryReading.hpp"
#include "commands.hpp"
#include "messages.hpp"

int main(int argv, char** argc)
{
  bazhenov::dictionaries storage;
  std::ifstream file;
  if (argv >= 2) {
    for (int i = 1; i < argv; ++i) {
      file.open(argc[i]);
      if (file.is_open()) {
        storage.insert(std::make_pair(bazhenov::createName(argc[i]), bazhenov::readDictionary(file)));
      } else {
        std::cerr << "Dictionary with name " << argc[i] << " cannot be created!";
      }
      file.close();
    }
  }

  std::map< std::string, std::function< void(bazhenov::dictionaries&) > > commands
  {
    {"print", bazhenov::print_t(std::cin, std::cout)},
    {"count", bazhenov::count_t(std::cin, std::cout)},
    {"clear", bazhenov::clear_t(std::cin)},
    {"find", bazhenov::find_t(std::cin, std::cout)},
    {"most", bazhenov::upper_t(std::cin, std::cout, "most")},
    {"least", bazhenov::upper_t(std::cin, std::cout, "least")},
    {"search_pref", bazhenov::search_pref_t(std::cin, std::cout)},
    {"search", bazhenov::search_t(std::cin, std::cout)},
    {"add", bazhenov::add_t(std::cin, std::cout)},
    {"diff", bazhenov::diff_t(std::cin, std::cout)}
  };

  std::string command = "";
  while (std::cin >> command) {
    try {
      commands.at(command)(storage);
    } catch (const std::exception& ex) {
      bazhenov::printInvalidCommand(std::cout) << "\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
