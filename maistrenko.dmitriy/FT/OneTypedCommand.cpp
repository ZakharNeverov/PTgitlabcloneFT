#include "OneTypedCommand.hpp"
#include "IOActions.hpp"
#include "CommandMessages.hpp"
#include "DictionaryActions.hpp"
#include <algorithm>
#include <iterator>

void maistrenko::createDict(std::string& commandLine, std::ostream& out, Dicts& dest)
{
  std::string dictName = "";
  if (getNextWord(commandLine, ' ', dictName) && commandLine.empty())
  {
    if (dest.find(dictName) == dest.end())
    {
      dest.insert({dictName, Dict()});
    }
    else
    {
      notifyExistingDict(out);
    }
  }
  else
  {
    notifyInvalidArguments(out);
  }
}

void maistrenko::getDict(std::string& commandLine, std::istream& in, std::ostream& out, Dicts& dest)
{
  std::string dictName = "";
  if (!getNextWord(commandLine, ' ', dictName) || !commandLine.empty())
  {
    notifyInvalidArguments(out);
    return;
  }
  if (dest.find(dictName) != dest.end())
  {
    notifyExistingDict(out);
    return;
  }

  Dict tempDict = Dict();
  getDictFromStream(in, tempDict);
  dest.insert({dictName, tempDict});
}

void maistrenko::getTranslations(std::string& commandLine, std::ostream& out, const Dicts& dicts)
{
  std::string dictName = "";
  bool isCommandValid = getNextWord(commandLine, ' ', dictName);

  EngW engWord = "";
  isCommandValid = getNextWord(commandLine, ' ', engWord) && isCommandValid;

  if (!isCommandValid || !commandLine.empty())
  {
    notifyInvalidArguments(out);
    return;
  }
  if (dicts.find(dictName) == dicts.end())
  {
    notifyUnexistingDict(out);
    return;
  }
  Dict currentDict = dicts.at(dictName);
  if (currentDict.find(engWord) == currentDict.end())
  {
    notifyUnexistingWord(out);
    return;
  }

  outputTranslations(currentDict, engWord, out);
}

void maistrenko::outputExactDict(std::string& commandLine, std::ostream& out, const Dicts& dicts)
{
  std::string dictName = "";
  if (!getNextWord(commandLine, ' ', dictName) || !commandLine.empty())
  {
    notifyInvalidArguments(out);
    return;
  }
  if (dicts.find(dictName) == dicts.end())
  {
    notifyUnexistingDict(out);
    return;
  }
  outputDict(dicts.at(dictName), out);
}

void maistrenko::addNewWord(std::string& commandLine, std::ostream& out, Dicts& dicts)
{
  std::string dictName = "";
  if (!getNextWord(commandLine, ' ', dictName))
  {
    notifyInvalidArguments(out);
    return;
  }
  if (dicts.find(dictName) == dicts.end())
  {
    notifyUnexistingDict(out);
    return;
  }

  std::pair< EngW, RusS > dictElem = std::pair< EngW, RusS >();
  if (maistrenko::getWordFromString(commandLine, dictElem) && commandLine.empty())
  {
    maistrenko::addWord(dicts.at(dictName), dictElem);
  }
  else
  {
    notifyInvalidArguments(out);
  }
}

void maistrenko::deleteDict(std::string& commandLine, std::ostream& out, Dicts& dicts)
{
  std::string dictName = "";
  if (getNextWord(commandLine, ' ', dictName) && commandLine.empty())
  {
    if (dicts.find(dictName) == dicts.end())
    {
      notifyUnexistingDict(out);
    }
    else
    {
      dicts.erase(dictName);
    }
  }
  else
  {
    notifyInvalidArguments(out);
  }
}

void maistrenko::deleteWord(std::string& commandLine, std::ostream& out, Dicts& dicts)
{
  std::string dictName = "";
  bool isCommandValid = getNextWord(commandLine, ' ', dictName);

  EngW engWord = "";
  isCommandValid = getNextWord(commandLine, ' ', engWord) && isCommandValid;

  if (!isCommandValid || !commandLine.empty())
  {
    notifyInvalidArguments(out);
    return;
  }
  if (dicts.find(dictName) == dicts.end())
  {
    notifyUnexistingDict(out);
    return;
  }
  if (!maistrenko::removeWord(dicts.at(dictName), engWord))
  {
    notifyUnexistingWord(out);
  }
}

void maistrenko::deleteTranslation(std::string& commandLine, std::ostream& out, Dicts& dicts)
{
  std::string dictName = "";
  bool isInputGood = getNextWord(commandLine, ' ', dictName);

  std::string engWord = "";
  isInputGood = getNextWord(commandLine, ' ', engWord) && isInputGood;

  std::string rusWord = "";
  isInputGood = getNextWord(commandLine, ' ', rusWord) && isInputGood;

  if (!isInputGood || !commandLine.empty())
  {
    notifyInvalidArguments(out);
    return;
  }
  if (dicts.find(dictName) == dicts.end())
  {
    notifyUnexistingDict(out);
    return;
  }

  Dict& currentDict = dicts.at(dictName);
  if (currentDict.find(engWord) == currentDict.end())
  {
    notifyUnexistingWord(out);
    return;
  }
  if (!maistrenko::removeTranslate(currentDict, engWord, rusWord))
  {
    notifyUnexistingTranslation(out);
  }
}

void maistrenko::unionDictionaries(std::string& commandLine, std::ostream& out, Dicts& dicts)
{
  std::string newDictName = "";
  bool isCommandValid = getNextWord(commandLine, ' ', newDictName);

  std::string dictName1 = "";
  isCommandValid = getNextWord(commandLine, ' ', dictName1) && isCommandValid;
  std::string dictName2 = "";
  isCommandValid = getNextWord(commandLine, ' ', dictName2) && isCommandValid;

  if (!isCommandValid || !commandLine.empty())
  {
    notifyInvalidArguments(out);
    return;
  }
  if (dicts.find(newDictName) != dicts.end())
  {
    notifyExistingDict(out);
    return;
  }
  if (dicts.find(dictName1) == dicts.end() || dicts.find(dictName2) == dicts.end())
  {
    notifyUnexistingDict(out);
    return;
  }

  Dict tempDict = maistrenko::unionDicts(dicts.at(dictName1), dicts.at(dictName2));
  dicts.insert({newDictName, tempDict});
}

void maistrenko::intersectDictionaries(std::string& commandLine, std::ostream& out, Dicts& dicts)
{
  std::string newDictName = "";
  bool isCommandValid = getNextWord(commandLine, ' ', newDictName);

  std::string dictName1 = "";
  isCommandValid = getNextWord(commandLine, ' ', dictName1) && isCommandValid;
  std::string dictName2 = "";
  isCommandValid = getNextWord(commandLine, ' ', dictName2) && isCommandValid;

  if (!isCommandValid || !commandLine.empty())
  {
    notifyInvalidArguments(out);
    return;
  }
  if (dicts.find(newDictName) != dicts.end())
  {
    notifyExistingDict(out);
    return;
  }
  if (dicts.find(dictName1) == dicts.end() || dicts.find(dictName2) == dicts.end())
  {
    notifyUnexistingDict(out);
    return;
  }

  Dict tempDict = maistrenko::intersectDicts(dicts.at(dictName1), dicts.at(dictName2));
  dicts.insert({newDictName, tempDict});
}

void maistrenko::substractDictionaries(std::string& commandLine, std::ostream& out, Dicts& dicts)
{
  std::string newDictName = "";
  bool isCommandValid = getNextWord(commandLine, ' ', newDictName);

  std::string dictName1 = "";
  isCommandValid = getNextWord(commandLine, ' ', dictName1) && isCommandValid;
  std::string dictName2 = "";
  isCommandValid = getNextWord(commandLine, ' ', dictName2) && isCommandValid;

  if (!isCommandValid || !commandLine.empty())
  {
    notifyInvalidArguments(out);
    return;
  }
  if (dicts.find(newDictName) != dicts.end())
  {
    notifyExistingDict(out);
    return;
  }
  if (dicts.find(dictName1) == dicts.end() || dicts.find(dictName2) == dicts.end())
  {
    notifyUnexistingDict(out);
    return;
  }

  Dict tempDict = maistrenko::substractDicts(dicts.at(dictName1), dicts.at(dictName2));
  dicts.insert({newDictName, tempDict});
}

void maistrenko::loadDict(std::string& commandLine, std::ostream& out, Dicts& dicts)
{
  std::string newDictName = "";
  bool isCommandValid = getNextWord(commandLine, ' ', newDictName);

  std::string fileName = "";
  isCommandValid = getNextWord(commandLine, ' ', fileName) && isCommandValid;

  if (!isCommandValid || !commandLine.empty())
  {
    notifyInvalidArguments(out);
    return;
  }
  if (dicts.find(newDictName) != dicts.end())
  {
    notifyExistingDict(out);
    return;
  }

  Dict newDict = Dict();
  if (loadFromFile(fileName, newDict))
  {
    dicts.insert({newDictName, newDict});
  }
  else
  {
    notifyUnexistingFile(out);
  }
}

void maistrenko::saveDict(std::string& commandLine, std::ostream& out, const Dicts& dicts)
{
  std::string outDictName = "";
  bool isCommandValid = getNextWord(commandLine, ' ', outDictName);

  std::string fileName = "";
  isCommandValid = getNextWord(commandLine, ' ', fileName) && isCommandValid;

  if (!isCommandValid || !commandLine.empty())
  {
    notifyInvalidArguments(out);
    return;
  }
  if (dicts.find(outDictName) == dicts.end())
  {
    notifyUnexistingDict(out);
    return;
  }
  if (!saveInFile(fileName, dicts.at(outDictName)))
  {
    notifyInvalidFile(out);
  }
}
