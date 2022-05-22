#include "CommandSystem.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <functional>
#include <iterator>
#include <algorithm>
#include "shannon-fano.hpp"
#include "IOfile.hpp"

namespace {
  void createStreamsEncode(std::string input, std::string codeFile, std::string output)
  {
    std::ifstream code = gusarov::openFile(codeFile);
    gusarov::dict_t fano = gusarov::getDictionary(code);
    code.close();
    std::ifstream in = gusarov::openFile(input);
    std::ofstream out(output);
    gusarov::doEncode(in, out, fano);
    in.close();
    out.close();
  }
  void createStreamsDecode(std::string input, std::string codeFile, std::string output)
  {
    std::ifstream code = gusarov::openFile(codeFile);
    gusarov::reverseDict_t fano = gusarov::getReverseDictionary(code);
    code.close();
    std::ifstream in = gusarov::openFile(input);
    std::ofstream out(output);
    gusarov::doDecode(in, out, fano);
    in.close();
    out.close();
  }
  void add(std::ofstream& out, std::string file)
  {
    std::ifstream in(file);
    using iterFile = std::istreambuf_iterator< char >;
    std::copy(iterFile(in), iterFile(), std::ostream_iterator< char >(out));
  }
}

gusarov::Command::Command():
  commandList({
    {"encode", std::bind(&gusarov::Command::encode, this, std::ref(std::cin))},
    {"decode", std::bind(&gusarov::Command::decode, this, std::ref(std::cin))},
    {"code", std::bind(&gusarov::Command::getCodeFano, this, std::ref(std::cin))},
    {"frequency", std::bind(&gusarov::Command::printFrequency, this, std::ref(std::cin))},
    {"add", std::bind(&gusarov::Command::mergeTwoFiles, this, std::ref(std::cin))},
    {"print", std::bind(&gusarov::Command::print, this, std::ref(std::cin))},
    {"difference", std::bind(&gusarov::Command::compareSize, this, std::ref(std::cin))}})
{}

void gusarov::Command::encode(std::istream& in)
{
  std::string input;
  in >> nameFileIO{input};
  std::string codeFile;
  in >> nameFileIO{codeFile};
  std::string output;
  in >> nameFileIO{output};
  createStreamsEncode(input, codeFile, output);
}

void gusarov::Command::decode(std::istream& in)
{
  std::string input;
  in >> nameFileIO{input};
  std::string codeFile;
  in >> nameFileIO{codeFile};
  std::string output;
  in >> nameFileIO{output};
  createStreamsDecode(input, codeFile, output);
}

void gusarov::Command::getCodeFano(std::istream& in)
{
  std::string name;
  in >> nameFileIO{name};
  std::ifstream fileRead = openFile(name); 
  in >> nameFileIO{name};
  std::ofstream fileWrite(name);

  createCodeFano(fileRead, fileWrite);

  fileRead.close();
  fileWrite.close();
}

void gusarov::Command::printFrequency(std::istream& in)
{
  std::string name;
  in >> nameFileIO{name};
  std::ifstream fileRead = openFile(name);
  fanoAlphabet_t fanoAlphabet = getFrequencyForCode(fileRead);

  fanoAlphabet_t::iterator iter = fanoAlphabet.begin();
  while (iter != fanoAlphabet.end()) {
    std::cout << getSymbol((*iter).symbol) << " " << (*iter).freq << "\n";
    iter++;
  }
}

void gusarov::Command::mergeTwoFiles(std::istream& in)
{
  std::string fileName1 = "";
  in >> nameFileIO{fileName1};
  std::string codeFile1 = "";
  in >> nameFileIO{codeFile1};
  std::string fileName2 = "";
  in >> nameFileIO{fileName2};
  std::string codeFile2 = "";
  in >> nameFileIO{codeFile2};
  std::string codeFile3 = "";

  if (in.peek() != '\n') {
    in >> nameFileIO{codeFile3};
    createStreamsDecode(fileName1, codeFile1, codeFile3);
    std::ofstream out(fileName1, std::ios::trunc);
    add(out, codeFile3);
    createStreamsDecode(fileName2, codeFile2, codeFile3);
    add(out, codeFile3);
    out.close();
    codeFile2 = codeFile3;
  } else {
    createStreamsDecode(fileName1, codeFile1, codeFile2);
    std::ofstream out(fileName1, std::ios::trunc);
    add(out, codeFile2);
    add(out, fileName2);
    out.close();
  }

  std::ifstream fileRead(fileName1);
  std::ofstream fileWrite(codeFile2);
  createCodeFano(fileRead, fileWrite);
  fileRead.close();
  fileWrite.close();
}

void gusarov::Command::print(std::istream& in)
{
  std::string name;
  in >> nameFileIO{name};
  std::ifstream file = openFile(name);
  using iterFile = std::istreambuf_iterator< char >;
  std::copy(iterFile(file), iterFile(), std::ostream_iterator< char >(std::cout));
  std::cout << std::endl;
}

void gusarov::Command::compareSize(std::istream& in)
{
  std::string name;
  in >> nameFileIO{name};
  std::ifstream first = openFile(name);
  in >> nameFileIO{name};
  std::ifstream second = openFile(name);

  first.seekg(0, std::ios::end);
  size_t sizeFirst = first.tellg();
  second.seekg(0, std::ios::end);
  size_t sizeSecond = second.tellg();
  std::cout << "first file: " << sizeFirst << " bytes\n";
  std::cout << "second file: " << std::ceil(sizeSecond / 8) << " bytes\n";
}

void gusarov::Command::doCommand(std::string command)
{
  auto commandIter = commandList.find(command);
  if (commandIter != commandList.end()) {
    commandIter->second();
  } else{
    throw std::invalid_argument("Unknown command");
  }
  if (std::cin.rdstate() == std::ios::failbit) {
    throw std::invalid_argument("incorrect file format");
  }
}
