#include "Words-commands.hpp"
std::string akimov::getWord(std::string& text)
{
  if (text.empty())
  {
    return "<NONE>";
  }
  skipSpaces(text);
  std::string word{};
  std::size_t space = text.find(" ");
  while (space != std::string::npos)
  {
    word = text.substr(0, space);
    erasePuncts(word);
    text.erase(0, space);
    skipSpaces(text);
    if (isWord(word))
    {
      return word;
    }
    space = text.find(" ");
  }
  word = text;
  text = "";
  erasePuncts(word);
  return isWord(word) ? word : "<NONE>";
}
bool akimov::isWord(const std::string& word)
{
  if (word.empty())
  {
    return false;
  }
  if (isNum(word))
  {
    return true;
  }
  else if (hasNum(word))
  {
    return false;
  }
  if (isCorrect(word))
  {
    return true;
  }
  return false;
}
bool akimov::isNum(const std::string& word)
{
  for (std::size_t i = 0; i < word.length(); i++)
  {
    if (!(word[i] >= '0' && word[i] <= '9'))
    {
      return false;
    }
  }
  return true;
}
bool akimov::isLetter(char symbol)
{
  return symbol >= 'A' && symbol <= 'Z' || symbol >= 'a' && symbol <= 'z';
}
bool akimov::isLowercaseLetter(char symbol)
{
  return symbol >= 'a' && symbol <= 'z';
}
bool akimov::hasNum(const std::string& word)
{
  for (std::size_t i = 0; i < word.length(); i++)
  {
    if (word[i] >= '0' && word[i] <= '9')
    {
      return true;
    }
  }
  return false;
}
bool akimov::isCorrect(const std::string& word)
{
  if (!(isGoodDash(word) && isLetter(word[0])))
  {
    return false;
  }
  for (std::size_t i = 1; i < word.length(); i++)
  {
    if (word[i] != '-')
    {
      if (!(isLowercaseLetter(word[i])))
      {
        return false;
      }
    }
    else
    {
      return isCorrect(word.substr(i + 1, word.length()));
    }
  }
  return true;
}
bool akimov::isGoodDash(const std::string& word)
{
  std::size_t count = 0;
  unsigned short int minLen = 2;
  unsigned short int maxCount = 1;
  for (std::size_t i = 0; i < word.length(); i++)
  {
    if (word[i] != '-')
    {
      continue;
    }
    if (i < minLen || i > (word.length() - 1 - minLen))
    {
      return false;
    }
    count++;
    if (count > maxCount)
    {
      return false;
    }
  }
  return true;
}
void akimov::skipSpaces(std::string& text)
{
  while (text.length() > 0 && text[0] == ' ')
  {
    text.erase(0, 1);
  }
}
void akimov::erasePuncts(std::string& word)
{
  std::size_t last = word.length() - 1;
  std::string puncts = "!.,?";
  bool hasPuncts = puncts.find(word[last]) != std::string::npos;
  while (last >= 0 && hasPuncts)
  {
    word.erase(last, last);
    last--;
    hasPuncts = puncts.find(word[last]) != std::string::npos;
  }
}
