#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <windows.h>

#include "Commands.hpp"
#include "Dictionary.hpp"
#include "Utilites.hpp"

using map = std::map< std::string, std::list< std::string > >;

int main()
{
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);
  std::ifstream in;
  std::string inFileName = "";
  std::ofstream out;
  std::string outFileName = "";
  std::string checkFile = "";
  std::cout << "Enter file's name to read commands: ";
  std::cin >> inFileName;
  std::cout << "\nEnter file's name to output results: ";
  std::cin >> outFileName;
  std::cout << "\nEnter file's name to compare results: ";
  std::cin >> checkFile;
  try
  {
    in.open(inFileName);
    if (!in)
    {
      throw std::invalid_argument("Couldn't open the input file.");
    }
    out.open(outFileName);
    if (!out)
    {
      throw std::invalid_argument("Couldn't open the output file.");
    }
    while (!in.eof())
    {
      try
      {
        std::string command = "";
        std::string line = "";
        std::getline(in, line);
        command = roletskaya::getElem(line);
        if ((command == "findWordByTranslation") || (command == "findTranslation") || (command == "print") ||
            (command == "translateText") || (command == "insert") || (command == "delete"))
        {
          std::ifstream fin;
          std::string dictName = roletskaya::getElem(line);
          fin.open(dictName);
          roletskaya::Dictionary dictionary;
          dictionary.createDictionary(fin);
          fin.close();
          if (command == "findWordByTranslation")
          {
            dictionary.findWordByTranslation(line, out);
            continue;
          } else if (command == "findTranslation")
          {
            dictionary.findTranslation(line, out);
            continue;
          } else if (command == "print")
          {
            dictionary.print(out);
            out << '\n';
            continue;
          } else if (command == "translateText")
          {
            std::string fileWithText = "";
            in >> fileWithText;
            in.open(fileWithText);
            dictionary.translateText(in, out);
            in.close();
            out << '\n';
            continue;
          } else if (command == "insert")
          {
            if (dictionary.insert(line) == true)
            {
              out << "Word is inserted.\n";
            }
            continue;
          } else if (command == "delete")
          {
            if (dictionary.deleteKey(line) == true)
            {
              out << "Word is deleted.\n";
            }
            continue;
          }
        } else if ((command == "merge") || (command == "complement") || (command == "equals"))
        {
          std::vector< map > vector = roletskaya::getFiles(line);
          if (vector.size() < 2)
          {
            throw std::invalid_argument("Too few dictionaries.\n");
          }
          if (command == "merge")
          {
            roletskaya::Dictionary newDictionary;
            newDictionary.inputResult(roletskaya::merge(vector));
            newDictionary.print(out);
            out << '\n';
            continue;
          } else if (command == "complement")
          {
            roletskaya::Dictionary dictionary;
            dictionary.inputResult(roletskaya::complement(vector));
            dictionary.print(out);
            out << '\n';
            continue;
          } else if (command == "equals")
          {
            if (roletskaya::equals(vector) == true)
            {
              out << "Dictionaries are equal.\n";
            } else
            {
              out << "Dictionaries are not equal.\n";
            }
            continue;
          }
        } else
        {
          throw std::invalid_argument("No such command.\n");
        }
      } catch (const std::exception& error)
      {
        out << error.what();
      }
    }
    in.close();
    out.close();
    if (roletskaya::checkResults(outFileName, checkFile) == false)
    {
      throw std::logic_error("Results are incorrect.\n");
    } else
    {
      std::cout << "Results are correct.\n";
    }
  } catch (const std::exception& error)
  {
    std::cerr << error.what() << '\n';
    return -1;
  }
  return 0;
}
