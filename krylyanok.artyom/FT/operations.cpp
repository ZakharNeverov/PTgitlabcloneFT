#include "operations.hpp"
#include <iostream>
#include "hash-dictionary.hpp"

void krylyanok::makeDict(std::string& nameFile, std::vector< std::string >& temp)
{
  std::ifstream file(nameFile);
  if (!file.is_open())
  {
    throw std::logic_error("");
  }
  std::string word;
  while (file >> word)
  {
    temp.push_back(word);
  }
  correctVector(temp);
  file.close();
}

void krylyanok::correctVector(std::vector< std::string >& vec)
{
  for (int i = 0; i < vec.size(); i++)
  {
    if (!correctString(vec.at(i)))
    {
      vec.erase(vec.begin() + i--);
    }
  }
}

bool krylyanok::correctString(std::string& string)
{
  const int DIFFERENCE_UPPERCASE_AND_LOWERCASE = 'A' - 'a';
  const int MAX_WORD_SIZE = 45;

  for (int checkIndex = 0; checkIndex < string.size(); ++checkIndex)
  {
    if (string.size() <= MAX_WORD_SIZE)
    {
      if (std::islower(string.at(checkIndex)) == 0)
      {
        if (std::isupper(string.at(checkIndex)) == 0)
        {
          return false;
        }
        else
        {
          string.at(checkIndex) -= DIFFERENCE_UPPERCASE_AND_LOWERCASE;
        }
      }
    }
  }
  return true;
}
