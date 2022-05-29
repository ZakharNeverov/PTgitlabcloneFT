#include "cross_references.hpp"

#include <algorithm>
#include <sstream>

bool shulgin::operator==(const CrossReferences& a, const CrossReferences& b)
{
  return a.name == b.name;
}

std::istream& shulgin::operator>>(std::istream& in, CrossReferences& data)
{
  for (size_t lineNumber = 1; !in.eof(); lineNumber++)
  {
    std::string line;
    std::getline(in, line);
    if (!line.empty())
    {
      std::stringstream stream(line);
      std::string word;
      while (stream >> word)
      {
        correctWord(word);
        if (!word.empty())
        {
          data.dict[word][*data.texts.begin()].insert(lineNumber);
        }
      }
    }
  }
  return in;
}

std::ostream& shulgin::operator<<(std::ostream& out, const CrossReferences& data)
{
  using ostreamIter = std::ostream_iterator< std::string >;
  std::transform(data.dict.cbegin(), data.dict.cend(), ostreamIter(out, "\n"),
    convertWordRefToString);
  return out;
}

std::string shulgin::convertWordRefToString(const WordAndTexts& item)
{
  std::ostringstream out;
  out << '<' << item.first << ">:\n";
  using ostringstreamIter = std::ostream_iterator< std::string >;
  std::transform(item.second.cbegin(), item.second.cend(), ostringstreamIter(out, "\n"),
    convertTextRefToString);
  return out.str();
}

std::string shulgin::convertTextRefToString(const TextAndLinesList& item)
{
  std::ostringstream out;
  out << item.first << ": ";
  using ostringstreamIter = std::ostream_iterator< size_t >;
  std::copy(item.second.cbegin(), item.second.cend(), ostringstreamIter(out, " "));
  return out.str();
}

void shulgin::correctWord(std::string& word)
{
  for (size_t i = 0; i < 3 && i < word.size(); i++)
  {
    if (ispunct(word[0]))
    {
      if (word.size() > 1)
      {
        word = word.substr(1, word.size() - 1);
      }
      else
      {
        word = "";
      }
    }
    if (word.size() > 1 && ispunct(word[word.size() - 1]))
    {
      word = word.substr(0, word.size() - 1);
    }
  }
  if (word.size() > 1 && isupper(word[0]) && islower(word[1]))
  {
    word[0] = tolower(word[0]);
  }
}
