#include "GetWord.hpp"

malakhov::GetWord::GetWord(const std::string& source):
  ref(source),
  wordBegin(source.begin())
{}

bool malakhov::GetWord::ended()
{
  while (*wordBegin == ' ')
  {
    ++wordBegin;
  }
  return wordBegin == ref.end();
}

std::string malakhov::GetWord::get()
{
  while (*wordBegin == ' ')
  {
    ++wordBegin;
  }

  std::string result;
  for (std::string::const_iterator it = wordBegin; (*it != ' ') && (it != ref.end()); ++it)
  {
    result += *it;
  }
  return result;
}

void malakhov::GetWord::toNext()
{
  while (*wordBegin == ' ')
  {
    ++wordBegin;
  }
  while ((*wordBegin != ' ') && (wordBegin != ref.end()))
  {
    ++wordBegin;
  }
}
