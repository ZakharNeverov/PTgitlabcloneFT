#include "commands.hpp"

#include <fstream>
#include <numeric>

#include "stream_guard.hpp"

bool shulgin::isName(const CrossReferences& data, const std::string& name)
{
  return data.name == name;
}

void shulgin::doReadCmd(RefDicts& data, std::istream& in)
{
  std::string fileName, dictName;
  in >> fileName >> dictName;
  if (!checkStreamOn(in, '\n'))
  {
    throw std::logic_error("Error, invalid command!");
  }
  std::fstream file(fileName);
  if (!file.is_open())
  {
    throw std::logic_error("Error, can not open the file <" + fileName + ">!");
  }
  CrossReferences input{ CrossReferencesDict(), dictName, Texts{ fileName } };
  file >> input;
  file.close();
  auto it = std::find(data.begin(), data.end(), input);
  if (it != data.end())
  {
    it->dict = uniteRefDicts(it->dict, input.dict);
    it->texts = uniteTexts(it->texts, input.texts);
  }
  else
  {
    data.push_back(input);
  }
}

void shulgin::doPrintCmd(const RefDicts& data, std::istream& in, std::ostream& out)
{
  std::string dictName;
  in >> dictName;
  if (!checkStreamOn(in, '\n'))
  {
    throw std::logic_error("Error, invalid command!");
  }
  auto it = detail::tryFindInRefDicts(data, dictName);
  out << *it;
}

void shulgin::doPrintTextCmd(std::istream& in, std::ostream& out)
{
  std::string fileName;
  in >> fileName;
  if (!checkStreamOn(in, '\n'))
  {
    throw std::logic_error("Error, invalid command!");
  }
  std::fstream file(fileName);
  if (!file.is_open())
  {
    throw std::logic_error("Error, can not open the file <" + fileName + ">!");
  }
  using ostreamIter = std::ostream_iterator< char >;
  using istreamIter = std::istream_iterator< char >;
  shulgin::StreamGuard guard(in);
  file >> std::noskipws;
  bool isNewLine = false;
  size_t line = 1;
  out << line << ") ";
  std::transform(istreamIter(file), istreamIter(), ostreamIter(out),
    [&](char x)
    {
      if (isNewLine)
      {
        out << ++line << ") ";
        isNewLine = false;
      }
      if (x == '\n')
      {
        isNewLine = true;
      }
      return x;
    });
  file.close();
  out << "\n\n";
}

void shulgin::doFindCmd(const RefDicts& data, std::istream& in, std::ostream& out)
{
  std::string dictName, option;
  in >> dictName >> option;
  if (checkStreamOn(in, '\n'))
  {
    auto it = detail::tryFindInRefDicts(data, dictName);
    if (isNumber(option))
    {
      TextWordsDict words = findCertainWords(*it, std::stoul(option));
      if (words.empty())
      {
        throw std::logic_error("Error, line <" + option + "> was not founded in <" + dictName + ">!");
      }
      out << "Found words of line <" + option + "> in <" + dictName + ">:\n";
      out << words;
    }
    else
    {
      TextLinesDict lines = findCertainLines(*it, option);
      if (lines.empty())
      {
        throw std::logic_error("Error, word <" + option + "> was not founded in <" + dictName + ">!");
      }
      out << "Found lines with word <" + option + "> in <" + dictName + ">:\n";
      out << lines;
    }
  }
  else
  {
    in.clear();
    std::string textName;
    in >> textName;
    if (!checkStreamOn(in, '\n'))
    {
      throw std::logic_error("Error, invalid command!");
    }
    auto it = detail::tryFindInRefDicts(data, dictName);
    if (it->texts.find(textName) == it->texts.cend())
    {
      throw std::logic_error("Error, can not find the text <" + textName + "> in <" + dictName + ">!");
    }
    if (isNumber(option))
    {
      Words words = findCertainWordsInText(*it, std::stoul(option), textName);
      if (words.empty())
      {
        throw std::logic_error("Error, line <" + option + "> was not founded in text <" + textName +
          "> of <" + dictName + ">!");
      }
      out << "Found words of line <" + option + "> in <" + dictName + ">, text <" + textName + ">: ";
      out << words << '\n';
    }
    else
    {
      Lines lines = findCertainLinesInText(*it, option, textName);
      if (lines.empty())
      {
        throw std::logic_error("Error, word <" + option + "> was not founded in text <" + textName +
          "> of <" + dictName + ">!");
      }
      out << "Found lines with word <" + option + "> in <" + dictName + ">, text <" + textName + ">: ";
      out << lines << '\n';
    }
  }
  out << '\n';
}

shulgin::TextWordsDict shulgin::findCertainWords(const CrossReferences& data, size_t line)
{
  using namespace std::placeholders;
  return std::accumulate(data.dict.cbegin(), data.dict.cend(), TextWordsDict(),
    std::bind(accumulateWordsWithLine, _1, _2, line));
}

shulgin::TextWordsDict& shulgin::accumulateWordsWithLine(TextWordsDict& data, const WordAndTexts& item, size_t line)
{
  std::for_each(item.second.cbegin(), item.second.cend(),
    [&](const TextAndLinesList& pair)
    {
      auto foundIt = pair.second.find(line);
      if (foundIt != pair.second.cend())
      {
        auto it = data.find(pair.first);
        if (it != data.end())
        {
          (*it).second.push_back(item.first);
        }
        else
        {
          data[pair.first] = std::vector< std::string >{ item.first };
        }
      }
    });
  return data;
}

shulgin::TextLinesDict shulgin::findCertainLines(const CrossReferences& data, const std::string& word)
{
  TextLinesDict tmp;
  auto wordIt = data.dict.find(word);
  if (wordIt != data.dict.cend())
  {
    std::for_each(wordIt->second.cbegin(), wordIt->second.cend(),
      [&tmp](const TextAndLinesList& item)
      {
        std::copy(item.second.cbegin(), item.second.cend(), std::back_inserter(tmp[item.first]));
      });
  }
  return tmp;
}

shulgin::Words shulgin::findCertainWordsInText(const CrossReferences& data, size_t line,
  const std::string& textName)
{
  using namespace std::placeholders;
  return std::accumulate(data.dict.cbegin(), data.dict.cend(), Words(),
    std::bind(accumulateWordsWithLineInText, _1, _2, line, textName));
}

shulgin::Words& shulgin::accumulateWordsWithLineInText(Words& data,
  const WordAndTexts& item, size_t line, const std::string& textName)
{
  auto textIt = item.second.find(textName);
  if (textIt != item.second.cend())
  {
    if (textIt->second.find(line) != textIt->second.cend())
    {
      data.push_back(item.first);
    }
  }
  return data;
}

shulgin::Lines shulgin::findCertainLinesInText(const CrossReferences& data,
  const std::string& word, const std::string& textName)
{
  Lines tmp;
  auto wordIt = data.dict.find(word);
  if (wordIt != data.dict.cend())
  {
    auto textIt = wordIt->second.find(textName);
    if (textIt != wordIt->second.cend())
    {
      std::copy(textIt->second.cbegin(), textIt->second.cend(), std::back_inserter(tmp));
    }
  }
  return tmp;
}

void shulgin::doRemoveCmd(RefDicts& data, std::istream& in, std::ostream& out)
{
  std::string dictName;
  in >> dictName;
  if (checkStreamOn(in, '\n'))
  {
    auto it = detail::tryFindInRefDicts(data, dictName);
    data.erase(it);
  }
  else
  {
    in.clear();
    std::string option;
    in >> option;
    if (checkStreamOn(in, '\n'))
    {
      auto it = detail::tryFindInRefDicts(data, dictName);
      if (isNumber(option))
      {
        if (!removeLine(*it, std::stoul(option)))
        {
          throw std::logic_error("Error, line <" + option + "> was not founded in <" + dictName + ">!");
        }
      }
      else
      {
        if (!removeWord(*it, option))
        {
          throw std::logic_error("Error, word <" + option + "> was not founded in <" + dictName + ">!");
        }
      }
    }
    else
    {
      in.clear();
      std::string textName;
      in >> textName;
      if (!checkStreamOn(in, '\n'))
      {
        throw std::logic_error("Error, invalid command!");
      }
      auto it = detail::tryFindInRefDicts(data, dictName);
      if (it->texts.find(textName) == it->texts.cend())
      {
        throw std::logic_error("Error, can not find the text <" + textName + "> in <" + dictName + ">!");
      }
      if (isNumber(option))
      {
        if (!removeLine(*it, std::stoul(option), textName))
        {
          throw std::logic_error("Error, line <" + option + "> was not founded in <" + dictName +
            ">, <" + textName + ">!");
        }
      }
      else
      {
        if (!removeWord(*it, option, textName))
        {
          throw std::logic_error("Error, word <" + option + "> was not founded in <" + dictName +
            ">, <" + textName + ">!");
        }
      }
    }
  }
}

bool shulgin::removeLine(CrossReferences& data, size_t line, const std::string& textName)
{
  bool isRemoved = false;
  for (auto wordIt = data.dict.begin(); wordIt != data.dict.end(); wordIt++)
  {
    if (!textName.empty())
    {
      auto textIt = wordIt->second.find(textName);
      if (textIt != wordIt->second.end())
      {
        auto lineIt = textIt->second.find(line);
        if (lineIt != textIt->second.end())
        {
          textIt->second.erase(lineIt);
          isRemoved = true;
          if (textIt->second.empty())
          {
            wordIt->second.erase(textIt);
          }
        }
      }
    }
    else
    {
      for (auto textIt = wordIt->second.begin(); textIt != wordIt->second.end(); textIt++)
      {
        auto lineIt = textIt->second.find(line);
        if (lineIt != textIt->second.end())
        {
          textIt->second.erase(lineIt);
          isRemoved = true;
          if (textIt->second.empty())
          {
            textIt = wordIt->second.erase(textIt);
            if (wordIt->second.empty())
            {
              break;
            }
            textIt--;
          }
        }
      }
    }
    if (wordIt->second.empty())
    {
      wordIt = data.dict.erase(wordIt);
      if (data.dict.empty())
      {
        break;
      }
      wordIt--;
    }
  }
  return isRemoved;
}

bool shulgin::removeWord(CrossReferences& data, const std::string& word, const std::string& textName)
{
  bool isRemoved = false;
  auto wordIt = data.dict.find(word);
  if (wordIt != data.dict.end())
  {
    if (!textName.empty())
    {
      auto textIt = wordIt->second.find(textName);
      if (textIt != wordIt->second.end())
      {
        wordIt->second.erase(textIt);
        isRemoved = true;
        if (wordIt->second.empty())
        {
          wordIt = data.dict.erase(wordIt);
        }
      }
    }
    else
    {
      data.dict.erase(wordIt);
      isRemoved = true;
    }
  }
  return isRemoved;
}

void shulgin::doRemoveTextCmd(RefDicts& data, std::istream& in, std::ostream& out)
{
  std::string dictName, textName;
  in >> dictName >> textName;
  if (!checkStreamOn(in, '\n'))
  {
    throw std::logic_error("Error, invalid command!");
  }
  auto it = detail::tryFindInRefDicts(data, dictName);
  auto textIt = it->texts.find(textName);
  if (textIt == it->texts.cend())
  {
    throw std::logic_error("Error, can not find the text <" + textName + "> in <" + dictName + ">!");
  }
  it->texts.erase(textIt);
  if (it->texts.empty())
  {
    data.erase(it);
  }
  else
  {
    removeText(*it, textName);
  }
}

void shulgin::removeText(CrossReferences& data, const std::string& textName)
{
  for (auto wordIt = data.dict.begin(); wordIt != data.dict.end(); wordIt++)
  {
    auto textIt = wordIt->second.find(textName);
    if (textIt != wordIt->second.end())
    {
      wordIt->second.erase(textIt);
    }
    if (wordIt->second.empty())
    {
      wordIt = data.dict.erase(wordIt);
      if (data.dict.empty())
      {
        break;
      }
      wordIt--;
    }
  }
}

void shulgin::doUniteCmd(RefDicts& data, std::istream& in, std::ostream& out)
{
  std::string firstDictName, secondDictName, newDictName;
  in >> firstDictName >> secondDictName >> newDictName;
  if (!checkStreamOn(in, '\n'))
  {
    throw std::logic_error("Error, invalid command!");
  }
  auto firstIt = detail::tryFindInRefDicts(data, firstDictName);
  auto secondIt = detail::tryFindInRefDicts(data, secondDictName);
  if (newDictName == firstDictName)
  {
    firstIt->dict = uniteRefDicts(firstIt->dict, secondIt->dict);
    firstIt->texts = uniteTexts(firstIt->texts, secondIt->texts);
  }
  else if (newDictName == secondDictName)
  {
    secondIt->dict = uniteRefDicts(firstIt->dict, secondIt->dict);
    secondIt->texts = uniteTexts(firstIt->texts, secondIt->texts);
  }
  else
  {
    using namespace std::placeholders;
    auto anotherIt = std::find_if(data.begin(), data.end(), std::bind(isName, _1, newDictName));
    if (anotherIt != data.end())
    {
      anotherIt->dict = uniteRefDicts(firstIt->dict, secondIt->dict);
      anotherIt->texts = uniteTexts(firstIt->texts, secondIt->texts);
    }
    else
    {
      CrossReferences newElement;
      newElement.dict = uniteRefDicts(firstIt->dict, secondIt->dict);
      newElement.name = newDictName;
      newElement.texts = uniteTexts(firstIt->texts, secondIt->texts);
      data.push_back(newElement);
    }
  }
}

shulgin::CrossReferencesDict shulgin::uniteRefDicts(const CrossReferencesDict& a, const CrossReferencesDict& b)
{
  CrossReferencesDict newDict;
  auto itA = a.cbegin();
  auto itB = b.cbegin();
  while (itA != a.cend() && itB != b.cend())
  {
    if (itA->first == itB->first)
    {
      newDict.insert(WordAndTexts(itA->first, uniteTextLinesDicts(itA->second, itB->second)));
      itA++;
      itB++;
    }
    else if (itA->first < itB->first)
    {
      newDict.insert(*itA);
      itA++;
    }
    else
    {
      newDict.insert(*itB);
      itB++;
    }
  }
  if (itA != a.cend() && itB == b.cend())
  {
    std::copy(itA, a.cend(), std::inserter(newDict, newDict.end()));
  }
  if (itA == a.cend() && itB != b.cend())
  {
    std::copy(itB, b.cend(), std::inserter(newDict, newDict.end()));
  }
  return newDict;
}

shulgin::TextLinesListDict shulgin::uniteTextLinesDicts(const TextLinesListDict& a, const TextLinesListDict& b)
{
  TextLinesListDict newDict;
  auto itA = a.cbegin();
  auto itB = b.cbegin();
  while (itA != a.cend() && itB != b.cend())
  {
    if (itA->first == itB->first)
    {
      newDict.insert(TextAndLinesList(itA->first, uniteLinesLists(itA->second, itB->second)));
      itA++;
      itB++;
    }
    else if (itA->first < itB->first)
    {
      newDict.insert(*itA);
      itA++;
    }
    else
    {
      newDict.insert(*itB);
      itB++;
    }
  }
  if (itA != a.cend() && itB == b.cend())
  {
    std::copy(itA, a.cend(), std::inserter(newDict, newDict.end()));
  }
  if (itA == a.cend() && itB != b.cend())
  {
    std::copy(itB, b.cend(), std::inserter(newDict, newDict.end()));
  }
  return newDict;
}

shulgin::LinesList shulgin::uniteLinesLists(const LinesList& a, const LinesList& b)
{
  LinesList newList(a);
  std::copy(b.cbegin(), b.cend(), std::inserter(newList, newList.end()));
  return newList;
}

shulgin::Texts shulgin::uniteTexts(const Texts& a, const Texts& b)
{
  Texts newTexts(a);
  std::copy(b.cbegin(), b.cend(), std::inserter(newTexts, newTexts.end()));
  return newTexts;
}

void shulgin::doCrossCmd(RefDicts& data, std::istream& in, std::ostream& out)
{
  std::string firstDictName, secondDictName, newDictName;
  in >> firstDictName >> secondDictName >> newDictName;
  if (!checkStreamOn(in, '\n'))
  {
    throw std::logic_error("Error, invalid command!");
  }
  auto firstIt = detail::tryFindInRefDicts(data, firstDictName);
  auto secondIt = detail::tryFindInRefDicts(data, secondDictName);
  if (newDictName == firstDictName)
  {
    firstIt->dict = crossRefDicts(firstIt->dict, secondIt->dict, firstIt->texts);
  }
  else if (newDictName == secondDictName)
  {
    secondIt->dict = crossRefDicts(firstIt->dict, secondIt->dict, secondIt->texts);
  }
  else
  {
    using namespace std::placeholders;
    auto anotherIt = std::find_if(data.begin(), data.end(), std::bind(isName, _1, newDictName));
    if (anotherIt != data.end())
    {
      anotherIt->dict = crossRefDicts(firstIt->dict, secondIt->dict, anotherIt->texts);
    }
    else
    {
      CrossReferences newElement;
      newElement.dict = crossRefDicts(firstIt->dict, secondIt->dict, newElement.texts);
      newElement.name = newDictName;
      data.push_back(newElement);
    }
  }
}

shulgin::CrossReferencesDict shulgin::crossRefDicts(const CrossReferencesDict& a,
  const CrossReferencesDict& b, Texts& texts)
{
  texts.clear();
  CrossReferencesDict newDict;
  auto itA = a.cbegin();
  auto itB = b.cbegin();
  while (itA != a.cend() && itB != b.cend())
  {
    if (itA->first == itB->first)
    {
      newDict.insert(WordAndTexts(itA->first, uniteTextLinesDicts(itA->second, itB->second)));
      std::transform(newDict[itA->first].cbegin(), newDict[itA->first].cend(), std::inserter(texts, texts.end()),
        [](const TextAndLinesList& item)
        {
          return item.first;
        });
      itA++;
      itB++;
    }
    else if (itA->first < itB->first)
    {
      itA++;
    }
    else
    {
      itB++;
    }
  }
  return newDict;
}
