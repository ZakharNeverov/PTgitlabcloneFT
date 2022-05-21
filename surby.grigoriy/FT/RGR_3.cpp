#include <iostream>
#include <string>
#include <fstream>

#include "Delimiter.hpp"
#include "EncodeDecode.hpp"

int main()
{
  std::locale::global(std::locale(".utf8"));
  std::string line;
  std::string def = "default";
  std::unordered_map < std::string, std::string > texts;
  while (std::getline(std::cin, line))
  {
    try
    {
      if (!std::cin.eof() && !line.empty())
      {
        std::string operation = getNextArg(line, 1);
        //addText C:\S1in.txt name
        if (operation == "addText")
        {
          std::string nameF = getNextArg(line, 1);
          std::ifstream in(nameF);
          std::string name = getNextArg(line, 1);
          std::string text;
          std::getline(in, text, '\0');
          texts[name] = text;
        }
        //getDictionary name C:\S1out.txt
        else if (operation == "getDictionary")
        {
          std::string name = getNextArg(line, 1);
          std::string fileOut = getNextArg(line, 1);
          std::ofstream out(fileOut);

          if (texts.find(name) == texts.end()) {
            std::cout << "text with name - " << name << " - exist\n";
          }
          else
          {
            out << getBestDict(texts[name]);
          }
        }
        //getText name C:\S1out.txt
        else if (operation == "getText")
        {
          std::string name = getNextArg(line, 1);
          if (texts.find(name) == texts.end()) {
            std::cout << "text with name - " << name << " - exist\n";
            break;
          }
          std::string fileOut = getNextArg(line, 1);
          if (fileOut[0] == '\0')
          {
            std::cout << texts[name] << '\n';
          }
          else
          {
            std::ofstream out(fileOut);
            out << texts[name];
          }
        }
        //encode C:\S1in.txt C:\S1out.txt name
        else if (operation == "encode")
        {
          std::string fileInText = getNextArg(line, 1);
          std::string fileInDict = getNextArg(line, 1);
          std::string name = getNextArg(line, 1);
          std::ifstream inText(fileInText);

          if (fileInDict == "default")
          {
            std::string text;
            std::getline(inText, text, '\0');
            texts[name] = encode(text, def);
          }
          else
          {
            std::ifstream inDict(fileInDict);
            std::string text;
            std::getline(inText, text, '\0');
            std::string dict;
            std::getline(inDict, dict, '\0');
            texts[name] = encode(text, dict);
          }
        }
        //decode name C:\S1out.txt name2
        else if (operation == "decode")
        {
          std::string name1 = getNextArg(line, 1);
          std::string fileInDict = getNextArg(line, 1);
          std::ifstream inDict(fileInDict);
          std::string name2 = getNextArg(line, 1);
          std::string dictionary;
          std::getline(inDict, dictionary, '\0');
          if (name2[0] == '\0')
          {
            texts[name1] = decode(texts[name1], dictionary);
          }
          else
          {
            texts[name2] = decode(texts[name1], dictionary);
          }
        }
        //encodeKey name default name2
        else if (operation == "encodeKey")
        {
          std::string name1 = getNextArg(line, 1);

          std::string fileInDict = getNextArg(line, 1);

          std::string name2 = getNextArg(line, 1);

          std::string answer;

          if (fileInDict == "default")
          {
            answer = encode(texts[name1], def);
          }
          else
          {
            std::ifstream inDict(fileInDict);
            std::string dict;
            std::getline(inDict, dict);
            answer = encode(texts[name1], dict);
          }

          if (name2[0] == '\0')
          {
            texts[name1] = answer;
          }
          else
          {
            texts[name2] = answer;
          }
        }
        //checkDifference name default
        else if (operation == "checkDifference")
        {
          std::string name = getNextArg(line, 1);
          std::string fileIn = getNextArg(line, 1);

          int memoryBefore = 0;
          int memoryAfter = 0;

          std::string text = texts[name];
          memoryBefore = text.size() * sizeof(char);

          if (fileIn == "default")
          {
            text = encode(text, def);
          }
          else
          {
            std::ifstream in(fileIn);
            std::string dict;
            std::getline(in, dict, '\0');
            text = encode(text, dict);
          }
          memoryAfter = text.size() / 8;

          std::cout << "memoryBefore  = " << memoryBefore << " bytes\nmemoryAfter = " << memoryAfter << " bytes" << std::endl;
          int dif = memoryBefore - memoryAfter;
          std::cout << "difference =  " << dif << " bytes" << std::endl;
          //std::cout << "memoryAfter = " << 100 * memoryBefore / memoryAfter << "% memoryBefore" << std::endl;
          std::cout << "compression = " << 100 - 100 * dif / memoryBefore << '%' << std::endl;
        }

        else
        {
          continue;
        }
        if (line[0] != '\0')
        {
          throw std::overflow_error("wrong input");
        }

      }
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return 2;
    }
  }
  return 0;
}
