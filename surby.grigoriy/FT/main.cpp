#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "Dictionary.hpp"

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
        std::string operation = surby::getNextArg(line, 1);
        //addText name C:\S1in.txt
        if (operation == "addText")
        {
          std::string name = surby::getNextArg(line, 1);
          std::string namefile = surby::getNextArg(line, 1);
          std::string text;

          if (namefile[0] == '\0')
          {
            std::getline(std::cin, text);
          }
          else
          {
            std::ifstream in(namefile);
            std::getline(in, text, '\0');
          }
          texts[name] = text;
        }
        //getDictionary name C:\S1out.txt
        else if (operation == "getDictionary")
        {
          std::string name = surby::getNextArg(line, 1);
          std::string namefile = surby::getNextArg(line, 1);


          if (texts.find(name) == texts.end()) {
            std::cout << "text with name - " << name << " - exist\n";
          }
          else
          {
            if (namefile[0] == '\0')
            {
              std::string answer = surby::getBestDict(texts[name]);
              std::cout << answer;
            }
            else
            {
              std::ofstream out(namefile);
              out << surby::getBestDict(texts[name]);
            }
          }
        }
        //getText name C:\S1out.txt
        else if (operation == "getText")
        {
          std::string name = surby::getNextArg(line, 1);

          if (texts.find(name) == texts.end())
          {
            std::cout << "text with name - " << name << " - exist\n";
            break;
          }
          std::string nanmefile = surby::getNextArg(line, 1);
          if (nanmefile[0] == '\0')
          {
            std::cout << texts[name] << '\n';
          }
          else
          {
            std::ofstream out(nanmefile);
            out << texts[name];
          }
        }
        //encode C:\S1in.txt C:\S1out.txt name
        else if (operation == "encode")
        {
          std::string fileInText = surby::getNextArg(line, 1);
          std::string fileInDict = surby::getNextArg(line, 1);
          std::string name = surby::getNextArg(line, 1);
          std::ifstream inText(fileInText);

          if (fileInDict == "default")
          {
            std::string text;
            std::getline(inText, text, '\0');
            texts[name] = surby::encode(text, def);
          }
          else
          {
            std::ifstream inDict(fileInDict);
            std::string text;
            std::getline(inText, text, '\0');
            std::string dict;
            std::getline(inDict, dict, '\0');
            texts[name] = surby::encode(text, dict);
          }
        }
        //decode name C:\S1out.txt name2
        else if (operation == "decode")
        {
          std::string name1 = surby::getNextArg(line, 1);
          std::string fileInDict = surby::getNextArg(line, 1);
          std::ifstream inDict(fileInDict);
          std::string name2 = surby::getNextArg(line, 1);
          std::string dictionary;
          std::getline(inDict, dictionary, '\0');
          if (name2[0] == '\0')
          {
            texts[name1] = surby::decode(texts[name1], dictionary);
          }
          else
          {
            texts[name2] = surby::decode(texts[name1], dictionary);
          }
        }
        //encodeKey name default name2
        else if (operation == "encodeKey")
        {
          std::string nameIn = surby::getNextArg(line, 1);
          std::string fileInDict = surby::getNextArg(line, 1);
          std::string nameOut = surby::getNextArg(line, 1);
          std::string answer;
          if (fileInDict == "default")
          {
            answer = surby::encode(texts[nameIn], def);
          }
          else
          {
            std::ifstream inDict(fileInDict);
            std::string dict;
            std::getline(inDict, dict);
            answer = surby::encode(texts[nameIn], dict);
          }
          if (nameOut[0] == '\0')
          {
            texts[nameIn] = answer;
          }
          else
          {
            texts[nameOut] = answer;
          }
        }
        //checkDifference name default
        else if (operation == "checkDifference")
        {
          std::string name = surby::getNextArg(line, 1);
          std::string fileIn = surby::getNextArg(line, 1);
          int memoryBefore = 0;
          int memoryAfter = 0;
          std::string text = texts[name];
          memoryBefore = text.size() * sizeof(char);
          if (fileIn == "default")
          {
            text = surby::encode(text, def);
          }
          else
          {
            std::ifstream in(fileIn);
            std::string dict;
            std::getline(in, dict, '\0');
            text = surby::encode(text, dict);
          }
          memoryAfter = text.size() / 8;
          std::cout << "memoryBefore  = " << memoryBefore << " bytes\nmemoryAfter = " << memoryAfter << " bytes" << std::endl;
          int dif = memoryBefore - memoryAfter;
          std::cout << "difference =  " << dif << " bytes" << std::endl;
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
