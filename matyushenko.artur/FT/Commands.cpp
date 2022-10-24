#include <iostream>
#include <map>
#include "Commands.hpp"
#include "functionsForSequences.hpp"
#include "printInformation.hpp"

namespace matyushenko {
  void print(const Commands::StrsDictT &strs, const Commands::ListOfArgs &args, std::ostream &out)
  {
    if (args.size() != 1) {
      throw std::logic_error("Not enough args in print");
    }
    std::string data = strs.at(*(args.begin()));
    if (data[data.find(' ') + 1] == '*') {
      data.erase(data.find(' ') + 1, 1);
    }
    out << data << '\n';
  }
  void printOld(const Commands::StrsDictT &strs, const Commands::ListOfArgs &args, std::ostream &out)
  {
    if (args.size() != 1) {
      throw std::logic_error("Not enough args in print");
    }
    std::string newStr;
    std::string newName = *args.begin();
    std::string str1;
    std::string str2;
    std::string name1;
    std::string name2;
    const std::string &listToFind = strs.at(newName);
    for (auto i = strs.begin(); i != strs.end(); ++i) {
      str1 = i->second;
      name1 = i->first;
      if (str1[0] != '*') {
        for (auto j = ++i; j != strs.end(); ++j) {
          str2 = j->second;
          name2 = j->first;
          if (str2[0] != '*') {
            newStr = matyushenko::findLongestSub(str1, str2);
            if (newStr == listToFind) {
              out << name1 << " " << str1 << " " << name2 << " " << str2;
              break;
            }
          }
        }
      }
    }
  }
  void findLongestSubsequence(Commands::StrsDictT &source, const matyushenko::Commands::ListOfArgs &args)
  {
    if (args.size() != 3) {
      throw std::logic_error("Not enough args in print");
    }
    auto arg = args.begin();
    const std::string newDictName = *arg;
    ++arg;
    std::string toInsert = "*";
    std::string result;
    const std::string &dict1 = source[*arg];
    ++arg;
    const std::string &dict2 = source[*arg];
    auto memory = dict2.begin();
    auto first = dict1.begin();
    size_t size_ = 0;
    while (dict1.size() != size_) {
      while (first != dict1.end()) {
        for (auto second = memory; second != dict2.end(); second++) {
          if (*first == *second) {
            memory = ++second;
            toInsert += *first;
            break;
          }
        }
        first++;
      }
      if (result.length() <= toInsert.length()) {
        result = toInsert;
      }
      toInsert = "*";
      memory = dict2.begin();
      size_++;
      first = dict1.begin() + size_;
    }
    if (result.length() <= toInsert.length()) {
      result = toInsert;
    }
    if (result.empty()) {
      std::cerr << "Нет пересечений между строками";
    }
    source.insert({newDictName, result});
  }
  void findLongest(const Commands::StrsDictT &strs, const Commands::ListOfArgs &args, std::ostream &out)
  {
    if (!args.empty()) {
      throw std::logic_error("Not enough args in print");
    }
    std::string str;
    std::string name;
    std::string max;
    size_t maxSize = std::numeric_limits< size_t >::min();
    for (auto i = strs.begin(); i != strs.end(); ++i) {
      str = i->second;
      name = i->first;
      if (str[0] == '*') {
        str.erase(0, 1);
        if (maxSize <= str.length()) {
          if (str.length() == maxSize) {
            max += name + " " + str;
          } else {
            maxSize = str.length();
            max = name + " " + str;
          }
        }
      }
    }
    out << max;
  }
  void findShortest(const Commands::StrsDictT &strs, const Commands::ListOfArgs &args, std::ostream &out)
  {
    if (!args.empty()) {
      throw std::logic_error("Not enough args in print");
    }
    std::string str;
    std::string name;
    size_t minSize = std::numeric_limits< size_t >::max();
    std::string min;
    for (auto i = strs.begin(); i != strs.end(); ++i) {
      str = i->second;
      name = i->first;
      if (str[0] == '*') {
        if (str[str.find(" ") + 1] == '*') {
          str.erase(str.find(" ") + 1, 1);
        }
        if (minSize <= str.length()) {
          if (str.length() == minSize) {
            min += name + " " + str;
          } else {
            minSize = str.length();
            min += " " + str;
          }
        }
      }
    }
    out << min;
  }
  void deleteSub(Commands::StrsDictT &source, const matyushenko::Commands::ListOfArgs &args)
  {
    if (args.size() != 1) {
      throw std::logic_error("Not enough args in print");
    }
    auto arg = args.begin();
    const std::string delName = *arg;
    source.erase(delName);
  }
  void findLength(const Commands::StrsDictT &strs, const Commands::ListOfArgs &args, std::ostream &out)
  {
    if (args.size() != 1) {
      throw std::logic_error("Not enough args in print");
    }
    auto arg = args.begin();
    const size_t length = std::stoi(*arg);
    std::string str;
    std::string name;
    for (auto i = strs.begin(); i != strs.end(); ++i) {
      str = i->second;
      name = i->first;
      if (str.length() == length) {
        std::cout << name << " " << str << "\n";
      }
    }
    out << name << " " << str << "\n";
  }
  using str = std::string;
  using funct = void (*)(const Commands::StrsDictT &, const Commands::ListOfArgs &, std::ostream &);
  std::map < str, funct > createInitList()
  {
    std::map < str, funct > initList{};
    initList.insert({"printSource", printOld});
    initList.insert({"print", print});
    initList.insert({"findOfLength", findLength});
    initList.insert({"findShortest", findShortest});
    initList.insert({"findLongest", findLongest});
    return initList;
  }
  matyushenko::Commands::Commands(StrsDictT &strs, std::ostream &out):
    strs_(strs),
    commands_({{"findLongestSub", findLongestSubsequence},
               {"remove",         deleteSub}}),
    constCommands_(createInitList()),
    out_(out)
  {}
  void matyushenko::Commands::call(const std::string& commandName, const ListOfArgs& args) const noexcept
  {
    try
    {
      auto command = constCommands_.find(commandName);
      if (command != constCommands_.end())
      {
        (*(*command).second)(strs_, args, out_);
      } else
      {
        auto command = commands_.find(commandName);
        if (command != commands_.end())
        {
          (*(*command).second)(strs_, args);
        } else
        {
          throw std::logic_error("No such command");
        }
      }
    } catch (const std::logic_error& e)
    {
      printInvalidCommand(out_) << '\n';
    }
  }
}
