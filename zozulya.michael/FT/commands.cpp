#include "commands.h"
#include <fstream>
#include <string>
#include <stdexcept>
#include <iostream>

void zozulya::readFileCMD(std::istream& in)
{
  std::string input;
  in >> input;
  getInputPath(input);
  Huffman huffman(input);
  huffman.readFile();
}

void zozulya::fillFrequencyCMD(std::istream& in)
{
  std::string input;
  in >> input;
  getInputPath(input);
  Huffman huffman(input);
  huffman.compress(input);
  huffman.fillFrequency();
}

void zozulya::printCodeDifferenceCMD(std::istream& in)
{
  std::string input;
  in >> input;
  getInputPath(input);
  Huffman huffman(input);
  huffman.compress(input);
  huffman.printCodeDifference();
}

void zozulya::getPath(std::string& from, std::string& to)
{
  from = "C:\\archive\\" + from + ".txt";
  to = "C:\\archive\\" + to;
}

std::string zozulya::getMsg(std::string& f1)
{
  std::ifstream in(f1);
  std::string msg;
  getline(in, msg);
  return msg;
}

void zozulya::getInputPath(std::string& input)
{
  input = "C:\\archive\\" + input + ".txt";
}

void zozulya::toCode(std::istream& in)
{
  std::string input;
  std::string output;
  in >> input >> output;
  getPath(input, output);
  Huffman compression(input);
  compression.compress(output);
}

void zozulya::compressionFiles(std::istream& in)
{
  std::string file1;
  std::string file2;
  in >> file1 >> file2;
  getInputPath(file1);
  getInputPath(file2);
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
  std::string outputMergeMsgFile = "C:\\archive\\mergeMessageCode.txt";
  
  in >> file1 >> file2;
  getInputPath(file1);
  getInputPath(file2);
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
  getInputPath(originFile);
  Huffman huffman(originFile);
  huffman.compress(originFile);

  std::ifstream file1(originFile);
  if (file1.fail())
  {
    std::cerr << "Wrong file name\n";
  }
  else
  {
    file1.seekg(0, std::ios::end);
    int sizeInputFile = 0;
    sizeInputFile = file1.tellg();

    std::cout << "File size before encoding: " << sizeInputFile << " bytes" << std::endl;

    std::ifstream file2(originFile + ".hff");
    file2.seekg(0, std::ios::end);
    int sizeAfterCode = 0;
    sizeAfterCode = file2.tellg();

    std::cout << "File size after encoding: " << sizeAfterCode << " байт" << std::endl;
  }
  
}

void zozulya::isFileEncrypted(std::istream& in)
{
  std::string filename;
  std::cin >> filename;
  std::string file = "C:\\archive\\" + filename + ".txt.hff";
  std::ifstream iff;
  iff.open(file);
  if (!iff)
  {
    std::cout << "Such a file was not encoded or you named the encoded file differently\n";
  }
  else
  {
    std::cout << "The code is in the file: " << file << std::endl;
  }
}
