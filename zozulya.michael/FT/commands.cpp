#include "commands.h"
#include <fstream>
#include <string>
#include <stdexcept>
#include <iostream>

void zozulya::readFileCMD(std::istream& in)
{
  std::string input;
  in >> input;
  Huffman huffman(input);
  huffman.readFile();
}

void zozulya::fillFrequencyCMD(std::istream& in)
{
  std::string input;
  in >> input;
  Huffman huffman(input);
  huffman.compress(input);
  huffman.fillFrequency();
}

void zozulya::printCodeDifferenceCMD(std::istream& in)
{
  std::string input;
  in >> input;
  std::ifstream file(input);
  if (file.fail())
  {
    throw std::logic_error("Error in file name\n");
  }
  Huffman huffman(input);
  huffman.compress(input);
  huffman.printCodeDifference();
}

std::string zozulya::getMsg(std::string& f1)
{
  std::ifstream in(f1);
  std::string msg;
  getline(in, msg);
  return msg;
}

void zozulya::toCode(std::istream& in)
{
  std::string input;
  std::string output;
  in >> input >> output;
  Huffman compression(input);
  compression.compress(output);
}

void zozulya::compressionFiles(std::istream& in)
{
  std::string file1;
  std::string file2;
  in >> file1 >> file2;
  Huffman compression1(file1);
  Huffman compression2(file2);
  compression1.compress(file1);
  compression2.compress(file2);
}

void zozulya::compressionAllFiles(std::istream& in)
{
  std::ofstream out;
  std::string file1;
  std::string file2;
  std::string mergeMessage;
  std::string outputMergeMsgFile = "mergeMessageCode.txt";
  in >> file1 >> file2;
  mergeMessage = getMsg(file1);
  mergeMessage += getMsg(file2);
  out.open(outputMergeMsgFile);
  out << mergeMessage;
  Huffman compression1(file1);
  Huffman compression2(file2);
  compression1.compress(outputMergeMsgFile);
  compression2.compress(outputMergeMsgFile);
}

void zozulya::delta(std::istream& in)
{
  std::string originFile;
  in >> originFile;
  Huffman huffman(originFile);
  huffman.compress(originFile);
  std::string msg1;
  std::string msg2;
  std::ifstream file1(originFile);
  if (file1.fail())
  {
    throw std::logic_error("Error in file name\n");
  }
  while (!file1.eof())
  {
    std::getline(file1, msg1);
  }
  int sizeInputFile = msg1.size() * sizeof(char);
  std::cout << "File size before encoding: " << sizeInputFile << " bytes" << std::endl;

  std::ifstream file2(originFile + ".hff");
  while (!file2.eof())
  {
    std::getline(file2, msg2);
  }
  int sizeAfterCode = msg2.size() * sizeof(char);
  std::cout << "File size after encoding: " << sizeAfterCode << " байт" << std::endl;
}

void zozulya::isFileEncrypted(std::istream& in)
{
  std::string filename;
  std::cin >> filename;
  std::string file = filename + ".txt.hff";
  std::ifstream iff;
  iff.open(file);
  std::cout << std::boolalpha << iff.is_open() << std::noboolalpha;
}
