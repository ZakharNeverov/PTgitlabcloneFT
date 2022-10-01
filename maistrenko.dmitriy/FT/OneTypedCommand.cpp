#include "OneTypedCommand.hpp"
#include "IOActions.hpp"
#include "CommandThrowers.hpp"
#include "DictionaryActions.hpp"

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
      raiseExistingDict();
    }
  }
  else
  {
    raiseInvalidArguments();
  }
}

void maistrenko::getDict(std::string& commandLine, std::istream& in, std::ostream& out, Dicts& dest)
{
  std::string dictName = "";
  if (!getNextWord(commandLine, ' ', dictName) || !commandLine.empty())
  {
    raiseInvalidArguments();
  }
  if (dest.find(dictName) != dest.end())
  {
    raiseExistingDict();
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
    raiseInvalidArguments();
  }
  if (dicts.find(dictName) == dicts.end())
  {
    raiseUnexistingDict();
  }
  Dict currentDict = dicts.at(dictName);
  if (currentDict.find(engWord) == currentDict.end())
  {
    raiseUnexistingWord();
  }

  outputTranslations(currentDict, engWord, out);
}

void maistrenko::outputExactDict(std::string& commandLine, std::ostream& out, const Dicts& dicts)
{
  std::string dictName = "";
  if (!getNextWord(commandLine, ' ', dictName) || !commandLine.empty())
  {
    raiseInvalidArguments();
  }
  if (dicts.find(dictName) == dicts.end())
  {
    raiseUnexistingDict();
  }
  outputDict(dicts.at(dictName), out);
}

void maistrenko::addNewWord(std::string& commandLine, std::ostream& out, Dicts& dicts)
{
  std::string dictName = "";
  if (!getNextWord(commandLine, ' ', dictName))
  {
    raiseInvalidArguments();
  }
  if (dicts.find(dictName) == dicts.end())
  {
    raiseUnexistingDict();
  }

  std::pair< EngW, RusS > dictElem = std::pair< EngW, RusS >();
  if (maistrenko::getWordFromString(commandLine, dictElem) && commandLine.empty())
  {
    maistrenko::addWord(dicts.at(dictName), dictElem);
  }
  else
  {
    raiseInvalidArguments();
  }
}

void maistrenko::deleteDict(std::string& commandLine, std::ostream& out, Dicts& dicts)
{
  std::string dictName = "";
  if (getNextWord(commandLine, ' ', dictName) && commandLine.empty())
  {
    if (dicts.find(dictName) == dicts.end())
    {
      raiseUnexistingDict();
    }
    else
    {
      dicts.erase(dictName);
    }
  }
  else
  {
    raiseInvalidArguments();
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
    raiseInvalidArguments();
  }
  if (dicts.find(dictName) == dicts.end())
  {
    raiseUnexistingDict();
  }
  maistrenko::removeWord(dicts.at(dictName), engWord);
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
    raiseInvalidArguments();
  }
  if (dicts.find(dictName) == dicts.end())
  {
    raiseUnexistingDict();
  }

  Dict& currentDict = dicts.at(dictName);
  if (currentDict.find(engWord) == currentDict.end())
  {
    raiseUnexistingWord();
  }
  maistrenko::removeTranslate(currentDict, engWord, rusWord);
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
    raiseInvalidArguments();
  }
  if (dicts.find(newDictName) != dicts.end())
  {
    raiseExistingDict();
  }
  if (dicts.find(dictName1) == dicts.end() || dicts.find(dictName2) == dicts.end())
  {
    raiseUnexistingDict();
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
    raiseInvalidArguments();
  }
  if (dicts.find(newDictName) != dicts.end())
  {
    raiseExistingDict();
  }
  if (dicts.find(dictName1) == dicts.end() || dicts.find(dictName2) == dicts.end())
  {
    raiseUnexistingDict();
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
    raiseInvalidArguments();
  }
  if (dicts.find(newDictName) != dicts.end())
  {
    raiseExistingDict();
  }
  if (dicts.find(dictName1) == dicts.end() || dicts.find(dictName2) == dicts.end())
  {
    raiseUnexistingDict();
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
    raiseInvalidArguments();
  }
  if (dicts.find(newDictName) != dicts.end())
  {
    raiseExistingDict();
  }

  Dict newDict = Dict();
  loadFromFile(fileName, newDict);
  dicts.insert({newDictName, newDict});
}

void maistrenko::saveDict(std::string& commandLine, std::ostream& out, const Dicts& dicts)
{
  std::string outDictName = "";
  bool isCommandValid = getNextWord(commandLine, ' ', outDictName);

  std::string fileName = "";
  isCommandValid = getNextWord(commandLine, ' ', fileName) && isCommandValid;

  if (!isCommandValid || !commandLine.empty())
  {
    raiseInvalidArguments();
  }
  if (dicts.find(outDictName) == dicts.end())
  {
    raiseUnexistingDict();
  }
  saveInFile(fileName, dicts.at(outDictName));
}
