#include "commands.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include <regex>
#include <cstddef>

#include "messages.h"

namespace {
  const std::string FILE_IS_NOT_OPENED = "Input file can't be opened";
  const std::string UNEXPECTED_COMMAND_ERROR = "Unexpected command";
  const std::string COLUMN_NAME_1 = "Word";
  const std::string COLUMN_NAME_2 = "Lines";
  const std::string ARGUMENT_ERROR = "Amount of arguments isn't corrected";
  const std::string DELETE_ERROR = "Name doesn't exist in global dictionary";
  const std::string FOUND_ERROR = "Can't find this dictionary";
  const std::string REGEX = "([a-zA-Z]+([-'][a-zA-Z]+)?)+";

  std::string printSpaces(size_t size)
  {
    return std::string(size, ' ');
  }

  std::ostream& printNamesOfColumns(size_t amountOfSpaces, std::ostream& out)
  {
    return out << COLUMN_NAME_1 << printSpaces(amountOfSpaces) << COLUMN_NAME_2;
  }
}

emelyanov::Command::Command(AllDataSets& data, std::istream& in, std::ostream& out):
  dataSets_(data),
  in_(in),
  out_(out),
  mapOfCommands_(
    {
      {"read", std::bind(&emelyanov::Command::read, this, std::placeholders::_1)},
      {"print", std::bind(&emelyanov::Command::printWord, this, std::placeholders::_1)},
      {"dictionary", std::bind(&emelyanov::Command::printDictionary, this, std::placeholders::_1)},
      {"delete", std::bind(&emelyanov::Command::deleteDict, this, std::placeholders::_1)},
      {"rename", std::bind(&emelyanov::Command::renameDict, this, std::placeholders::_1)},
      {"union", std::bind(&emelyanov::Command::unionDict, this, std::placeholders::_1)},
      {"intersect", std::bind(&emelyanov::Command::intersectDict, this, std::placeholders::_1)},
      {"complement", std::bind(&emelyanov::Command::complementDict, this, std::placeholders::_1)},
      {"contains", std::bind(&emelyanov::Command::contains, this, std::placeholders::_1)}
    })
{}

void emelyanov::Command::operator()(CommandArgs& args)
{
  std::string command = args.front();
  args.pop_front();
  try {
    mapOfCommands_.at(command)(args);
  }
  catch (const std::exception& e) {
    throw std::logic_error(UNEXPECTED_COMMAND_ERROR);
  }
}

void emelyanov::Command::read(CommandArgs& args)
{
  if (args.size() != 3) {
    throw std::logic_error(ARGUMENT_ERROR);
  }
  doRead(args);
}

void emelyanov::Command::printDictionary(CommandArgs& args)
{
  if (args.size() != 1) {
    throw std::logic_error(ARGUMENT_ERROR);
  }
  doPrintDictionary(args);
}

void emelyanov::Command::printWord(CommandArgs& args)
{
  if (args.size() == 1) {
    printWordFromAllDicts(args);
  } else if (args.size() == 2) {
    printWordFromOneDict(args);
  } else {
    throw std::logic_error(ARGUMENT_ERROR);
  }
}

void emelyanov::Command::complementDict(CommandArgs& args)
{
  if (args.size() != 3) {
    throw std::logic_error(ARGUMENT_ERROR);
  }
  doComplementDict(args);
}

void emelyanov::Command::intersectDict(CommandArgs& args)
{
  if (args.size() != 3) {
    throw std::logic_error(ARGUMENT_ERROR);
  }
  doIntersectDict(args);
}

void emelyanov::Command::unionDict(CommandArgs& args)
{
  if (args.size() != 3) {
    throw std::logic_error(ARGUMENT_ERROR);
  }
  doUnionDict(args);
}

void emelyanov::Command::renameDict(CommandArgs& args)
{
  if (args.size() != 2) {
    throw std::logic_error(ARGUMENT_ERROR);
  }
  doRenameDict(args);
}

void emelyanov::Command::deleteDict(CommandArgs& args)
{
  if (args.size() != 1) {
    throw std::logic_error(ARGUMENT_ERROR);
  }
  doDeleteDict(args);
}

void emelyanov::Command::contains(CommandArgs& args)
{
  if (args.size() != 1) {
    throw std::logic_error(ARGUMENT_ERROR);
  }
  doContains(args);
}

void emelyanov::Command::doRead(CommandArgs& args)
{
  DataSet dataSet;
  std::string dataSetName = args.front();
  args.pop_front();
  std::string inputFileName = args.front();
  args.pop_front();
  std::fstream inputFile(inputFileName);
  if (!inputFile.is_open()) {
    throw std::logic_error(FILE_IS_NOT_OPENED);
  }
  std::string outputFileName = args.front();
  args.pop_front();
  std::ofstream outputFile(outputFileName);
  if (!outputFile.is_open()) {
    throw std::logic_error(FILE_IS_NOT_OPENED);
  }
  const std::regex regex(REGEX);
  std::smatch smatch;
  for (size_t lineIndex = 1; !inputFile.eof(); ++lineIndex) {
    std::string data;
    std::getline(inputFile, data);
    const size_t indent = 5;
    outputFile << lineIndex << printSpaces(indent) << data << "\n";
    while (std::regex_search(data, smatch, regex)) {
      std::string word = smatch.str();
      std::transform(word.begin(), word.end(), word.begin(), ::tolower);
      if (dataSet.find(word) != dataSet.end()) {
        dataSet[word].insert(lineIndex);
      } else {
        std::set< size_t > set;
        set.insert(lineIndex);
        dataSet.insert(std::make_pair(word, set));
      }
      data = smatch.suffix().str();
    }
  }
  inputFile.close();
  dataSets_.insert(std::make_pair(dataSetName, dataSet));
}

void emelyanov::Command::doPrintDictionary(CommandArgs& args)
{
  std::string dataSetName = args.front();
  args.pop_front();
  auto currentDict = dataSets_.at(dataSetName);
  if (currentDict.empty()) {
    printEmpty(out_) << '\n';
    return;
  }
  size_t maxWordWidth = 5;
  for (auto&& el: currentDict) {
    maxWordWidth = std::max(el.first.length(), maxWordWidth);
  }
  const size_t indent = 5;
  printNamesOfColumns(maxWordWidth, out_) << '\n';
  for (auto&& el: currentDict) {
    const size_t currentWidth = maxWordWidth - el.first.length() + indent;
    out_ << el.first << printSpaces(currentWidth);
    using namespace std::placeholders;
    auto begin = el.second.begin();
    auto end = el.second.end();
    std::copy(begin, std::prev(end), std::ostream_iterator< size_t >(out_, " "));
    out_ << *std::prev(end) << '\n';
  }
}

void emelyanov::Command::printWordFromOneDict(CommandArgs& args)
{
  std::string dataSetName = args.front();
  args.pop_front();
  std::string word = args.front();
  args.pop_front();
  auto currentDict = dataSets_.at(dataSetName);
  if (currentDict.empty()) {
    printEmpty(out_) << '\n';
    return;
  }
  size_t maxWordWidth = word.length();
  const size_t indent = 5;
  printNamesOfColumns(maxWordWidth, out_) << '\n';
  out_ << word << printSpaces(indent);
  auto setWithValues = currentDict.at(word);
  using namespace std::placeholders;
  auto begin = setWithValues.begin();
  auto end = setWithValues.end();
  std::copy(begin, std::prev(end), std::ostream_iterator< size_t >(out_, " "));
  out_ << *std::prev(end) << '\n';
}

void emelyanov::Command::printWordFromAllDicts(CommandArgs& args)
{
  std::string word = args.front();
  args.pop_front();
  size_t maxWordWidth = word.length();
  const size_t indent = 5;
  printNamesOfColumns(maxWordWidth, out_) << '\n';
  out_ << word << printSpaces(indent - 1);
  std::set< size_t > newNumbers;
  using namespace std::placeholders;
  for (auto it = dataSets_.begin(); it != dataSets_.end(); ++it) {
    if (it->second.empty()) {
      continue;
    }
    auto currentIt = it->second.find(word);
    if (currentIt == it->second.end()) {
      continue;
    }
    auto begin = currentIt->second.begin();
    auto end = currentIt->second.end();
    std::copy(begin, end, std::inserter(newNumbers, newNumbers.begin()));
  }
  auto begin = newNumbers.begin();
  auto end = newNumbers.end();
  std::copy(begin, std::prev(end), std::ostream_iterator< size_t >(out_, " "));
  out_ << *std::prev(end);
}

void emelyanov::Command::doComplementDict(CommandArgs& args)
{
  DataSet result;
  std::string newDataSetName = args.front();
  args.pop_front();
  std::string dataSetName1 = args.front();
  args.pop_front();
  std::string dataSetName2 = args.front();
  args.pop_front();
  auto it1 = dataSets_.find(dataSetName1);
  auto it2 = dataSets_.find(dataSetName2);
  if (it1 == dataSets_.end() || it2 == dataSets_.end()) {
    printEmpty(out_);
    return;
  }
  for (auto&& data: it1->second) {
    if (it2->second.find(data.first) == it2->second.end()) {
      result.insert(std::make_pair(data.first, data.second));
    }
  }
  dataSets_.insert(std::make_pair(newDataSetName, result));
}

void emelyanov::Command::doIntersectDict(CommandArgs& args)
{
  emelyanov::DataSet result;
  std::string newDataSetName = args.front();
  args.pop_front();
  std::string dataSetName1 = args.front();
  args.pop_front();
  std::string dataSetName2 = args.front();
  args.pop_front();
  auto it1 = dataSets_.find(dataSetName1);
  auto it2 = dataSets_.find(dataSetName2);
  if (it1 == dataSets_.end() || it2 == dataSets_.end()) {
    printEmpty(out_);
    return;
  }
  for (auto&& data: it1->second) {
    if (it2->second.find(data.first) != it2->second.end()) {
      result.insert(std::make_pair(data.first, data.second));
    }
  }
  dataSets_.insert(std::make_pair(newDataSetName, result));
}

void emelyanov::Command::doUnionDict(CommandArgs& args)
{
  emelyanov::DataSet result;
  std::string newDataSetName = args.front();
  args.pop_front();
  std::string dataSetName1 = args.front();
  args.pop_front();
  std::string dataSetName2 = args.front();
  args.pop_front();
  auto it1 = dataSets_.find(dataSetName1);
  auto it2 = dataSets_.find(dataSetName2);
  if (it1 == dataSets_.end() || it2 == dataSets_.end()) {
    printEmpty(out_);
    return;
  }
  for (auto&& data: it1->second) {
    result.insert(std::make_pair(data.first, data.second));
  }
  for (auto&& data: it2->second) {
    if (result.find(data.first) == result.end()) {
      result.insert(std::make_pair(data.first, data.second));
    }
  }
  dataSets_.insert(std::make_pair(newDataSetName, result));
}

void emelyanov::Command::doRenameDict(CommandArgs& args)
{
  std::string oldDataSetName = args.front();
  args.pop_front();
  std::string newDataSetName = args.front();
  args.pop_front();
  auto it1 = dataSets_.find(oldDataSetName);
  auto it2 = dataSets_.find(newDataSetName);
  if (it1 == dataSets_.end() || it2 != dataSets_.end()) {
    printEmpty(out_);
    return;
  }
  dataSets_.insert(std::make_pair(newDataSetName, it1->second));
  dataSets_.erase(oldDataSetName);
}

void emelyanov::Command::doDeleteDict(CommandArgs& args)
{
  std::string dataSetName = args.front();
  args.pop_front();
  if (dataSets_.find(dataSetName) == dataSets_.end()) {
    throw std::logic_error(DELETE_ERROR);
  }
  dataSets_.erase(dataSetName);
}

void emelyanov::Command::doContains(CommandArgs& args)
{
  std::string dataSetName = args.front();
  args.pop_front();
  if (dataSets_.find(dataSetName) != dataSets_.end()) {
    printTrue(out_) << '\n';
  } else {
    printFalse(out_) << '\n';
  }
}
