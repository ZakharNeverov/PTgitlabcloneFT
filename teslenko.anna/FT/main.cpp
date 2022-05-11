#include <string>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <list>
#include <map>
#include <vector>
#include "text-analyzer.hpp"

namespace teslenko
{
  void analyzeText(const std::string& inFilename, std::ostream& out, const std::string& way)
  {
    TextAnalyzer textAnalyzer{ };
    if (way == "console")
    {
      textAnalyzer.printAnalysis(std::cout, inFilename);
    }
    else if (way == "file")
    {
      textAnalyzer.printAnalysis(out, inFilename);
    }
  }

  std::string getTheLineElem(std::string& line)
  {
    size_t nextSpace = line.find_first_of(' ', 0);
    std::string elem = line.substr(0, nextSpace);
    nextSpace = (nextSpace == line.npos) ? nextSpace : nextSpace + 1;
    line.erase(0, nextSpace);
    return elem;
  }
}

int main()
{
  std::ifstream in;
  std::string inFileName = "";
  std::ofstream out;
  std::string outFileName = "";
  try
  {
    std::cout << "Enter the name of the source data file including extension (e.g. input.txt): ";
    std::cin >> inFileName;
    in.open(inFileName);
    if (!in)
    {
      throw std::logic_error("Error, file not open\n");
    }
    std::cout << "Enter the file name where to output the result: ";
    std::cin >> outFileName;
    out.open(outFileName);
    if (!out)
    {
      throw std::logic_error("Error, file not open\n");
    }
    std::string line = "";
    while (!in.eof())
    {
      std::getline(in, line);
      std::string ch = teslenko::getTheLineElem(line);
      if (in && (ch != "1") && (ch != "2") && (ch != "3") && (ch != "4") && (ch != "5"))
      {
        throw std::logic_error("Command not found\n");
      }
      if (ch == "1" || ch == "5")
      {
        std::string fileDictianoryForAnalysis = teslenko::getTheLineElem(line);
        if (ch == "1")
        {
          std::string way = teslenko::getTheLineElem(line);
          teslenko::analyzeText(fileDictianoryForAnalysis, out, way);
        }
        else
        {
          teslenko::TextAnalyzer textAnalyzer{ };
          std::string word = teslenko::getTheLineElem(line);
          std::map< std::string, std::vector< int > > dictionary_ = { };
          dictionary_ = textAnalyzer.analyze(fileDictianoryForAnalysis);
          if (textAnalyzer.searchWord(dictionary_, word))
          {
            out << "Key-value pair present in dictionary\n";
          }
          else
          {
            out << "Key-value pair not present in dictionary\n";
          }
        }
      }
      else if (ch == "2" || ch == "3" || ch == "4")
      {
        teslenko::TextAnalyzer textAnalyzer{ };
        std::vector< std::map< std::string, std::vector< int > > > arrayDictionaries;
        std::string countFilesString = teslenko::getTheLineElem(line);
        size_t countFiles = std::stoi(countFilesString);
        if (!countFiles || (countFiles < 2))
        {
          throw std::logic_error("Not a number\n");
        }
        for (size_t i = 0; i < countFiles; ++i)
        {
          std::string fileWithDictionary = teslenko::getTheLineElem(line);
          std::map< std::string, std::vector< int > > dictionary_ = { };
          dictionary_ = textAnalyzer.analyze(fileWithDictionary);
          arrayDictionaries.push_back(dictionary_);
        }
        if (ch == "2")
        {
          textAnalyzer.uniteDictionaries(arrayDictionaries, out);
        }
        if (ch == "3")
        {
          textAnalyzer.subtractionDictionaries(arrayDictionaries, out);
        }
        if (ch == "4")
        {
          textAnalyzer.getIntersectionDictionaries(arrayDictionaries, out);
        }
      }
      out << "\n";
    }
    out.close();
    in.close();
    in.open(outFileName);
    std::ifstream f;
    std::string expectedResFile = "";
    std::cout << "Enter the name of the expected result file including extension (e.g. input.txt): ";
    std::cin >> expectedResFile;
    f.open(expectedResFile);
    if (!f)
    {
      throw std::logic_error("Error, file not open");
    }
    std::string line1 = "";
    std::string line2 = "";
    while (!in.eof() || !f.eof())
    {
      std::getline(in, line1);
      std::getline(f, line2);
      if (line1 != line2)
      {
        throw std::logic_error("ERROR_PROGRAM_NOT_CORRECTLY\n");
      }
    }
    if ((in.eof() && !f.eof()) || (!in.eof() && f.eof()))
    {
      throw std::logic_error("ERROR_PROGRAM_NOT_CORRECTLY\n");
    }
    in.close();
    f.close();
  }
  catch (const std::exception& ex)
  {
    std::cerr << ex.what() << '\n';
    return 1;
  }
  return 0;
}
