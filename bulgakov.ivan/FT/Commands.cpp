#include "Commands.hpp"
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <functional>
#include <vector>

namespace
{
  void correctWord(std::string& word)
  {
    std::transform(word.begin(), word.end(), word.begin(), std::tolower);
    std::string toErase = ".,:;?!-()";
    char first = word[0];
    char last = word[word.size() - 1];
    if (toErase.find(first) != std::string::npos)
    {
      word.erase(word.begin());
    }
    if (toErase.find(last) != std::string::npos)
    {
      word.erase(word.size() - 1);
    }
  }

  std::vector< std::string > getWords(std::istream& istr)
  {
    std::vector< std::string > words = {};
    std::string word = "";
    while (istr.get() != '\n')
    {
      istr >> word;
      words.push_back(word);
    }
    return words;
  }

  bool contains(std::string& mapName, bulgakov::maps_t& maps)
  {
    return maps.find(mapName) != maps.end();
  }
  bool containsWord(std::string& word, bulgakov::map_t& map)
  {
    return map.find(word) != map.end();
  }

}

void bulgakov::analyze(std::istream& stream, maps_t& maps)
{
  std::vector< std::string > words = getWords(stream);
  if (words.size() != 2)
  {
    throw std::logic_error("Invalid command.");
  }
  std::string fileName = words[1];
  std::ifstream in(fileName);
  if (!in.is_open())
  {
    throw std::logic_error("Error, file is not open.");
  }
  map_t map;
  std::string word = "";
  while (!in.eof())
  {
    in >> word;
    correctWord(word);
    map[word]++;
  }
  std::string mapName = words[0];
  maps[mapName] = map;
}

void bulgakov::remove(std::istream& stream, maps_t& maps)
{
  std::vector< std::string > words = getWords(stream);
  if (words.size() > 2 && words.size() < 1)
  {
    throw std::logic_error("Invalid command.");
  }
  if (words.size() == 2)
  {
    std::string dict = words[0];
    std::string remWord = words[1];
    if (contains(dict, maps))
    {
      map_t remDict = maps[dict];
      if (containsWord(remWord, remDict))
      {
        remDict.erase(remDict.find(remWord));
      }
      else
      {
        throw std::logic_error("Didn't have word.");
      }
    }
    else
    {
      throw std::logic_error("Didn't have dict.");
    }
  }
  else if (words.size() == 1)
  {
    std::string remDict = words[0];
    if (contains(remDict, maps))
    {
      maps.erase(maps.find(remDict));
    }
    else
    {
      throw std::logic_error("Didn't have dict.");
    }
  }
  else
  {
    throw std::logic_error("Didn't have args.");
  }
}

void bulgakov::mergeDir(std::istream& stream, maps_t& maps)
{
  std::vector< std::string > words = getWords(stream);
  if (words.size() != 3)
  {
    throw std::logic_error("Invalid command.");
  }
  std::string newDict = words[0];
  std::string firstDict = words[1];
  std::string secondDict = words[2];
  if (!contains(firstDict, maps) || !contains(secondDict, maps))
  {
    throw std::logic_error("Wrong nameDict or didn't have they");
  }
  map_t dict1 = maps[firstDict];
  map_t dict2 = maps[secondDict];
  map_t nDict;
  std::merge(dict1.begin(), dict1.end(), dict2.begin(), dict2.end(), std::inserter(nDict, nDict.end()));
  maps[newDict] = nDict;
}

void bulgakov::doUniqDict(std::istream& stream, maps_t& maps, cmp_func cmp)
{
  std::vector< std::string > words = getWords(stream);
  if (words.size() != 3)
  {
    throw std::logic_error("Invalid command.");
  }
  std::string newDict = words[0];
  std::string firstDict = words[1];
  std::string secondDict = words[2];
  if (!contains(firstDict, maps) || !contains(secondDict, maps))
  {
    throw std::logic_error("Wrong nameDict or didn't have they");
  }
  map_t dict1 = maps[firstDict];
  map_t dict2 = maps[secondDict];
  map_t nDict;
  using namespace std::placeholders;
  std::copy_if(dict1.begin(), dict1.end(), std::inserter(nDict, nDict.end()), std::bind(cmp, _1, dict2));
  maps[newDict] = nDict;
}

void bulgakov::doFreqDictRare(std::istream& stream, maps_t& maps, get_el getl)
{
  std::vector< std::string > words = getWords(stream);
  if (words.size() != 3)
  {
    throw std::logic_error("Invalid command.");
  }
  std::string newDict = words[0];
  std::string firstDict = words[1];
  std::string secondDict = words[2];
  if (!contains(firstDict, maps) || !contains(secondDict, maps))
  {
    throw std::logic_error("Wrong nameDict or didn't have they");
  }
  map_t dict1 = maps[firstDict];
  map_t dict2 = maps[secondDict];
  map_t nDict;
  map_t buff1 = dict1;
  map_t buff2 = dict2;
  node_t el1 = getl(buff1);
  node_t el2 = getl(buff2);
  nDict.insert(el1);
  nDict.insert(el2);
  maps[newDict] = nDict;
}

void bulgakov::searchWord(std::istream& stream, maps_t& maps, std::ostream& out)
{
  std::vector< std::string > words = getWords(stream);
  if (words.size() != 1)
  {
    throw std::logic_error("Invalid command.");
  }
  std::string word = words[0];
  bool flag = true;
  for (auto&& i: maps)
  {
    if (i.second.find(word) != i.second.end())
    {
      out << "In " << i.first << " has " << word << " in quantity " << (*i.second.find(word)).second;
      flag = false;
    }
  }
  if (flag)
  {
    out << "Dictionary doesn't have this word";
  }
}

void bulgakov::print(std::istream& stream, maps_t& maps, std::ostream& out)
{
  std::vector< std::string > words = getWords(stream);
  if (words.size() != 1)
  {
    throw std::logic_error("Invalid command.");
  }
  std::string name = words[0];
  if (name != "all")
  {
    if (!contains(name, maps))
    {
      throw std::logic_error("Wrong nameDict or didn't have they");
    }
    map_t dict = maps[name];
    for (auto&& i: dict)
    {
      out << i.first << ' ' << i.second << "; ";
    }
  }
  else
  {
    for (auto&& i: maps)
    {
      out << i.first << ' ';
      for (auto j = i.second.begin(); j != i.second.end(); j++)
      {
        out << (*j).first << ' ' << (*j).second << "; ";
      }
      out << '\n';
    }
  }
}

bool bulgakov::frequentCompare(node_t& first, node_t& second)
{
  return first.second < second.second;
}

bulgakov::node_t& bulgakov::getFreqEl(map_t& map)
{
  return *std::max_element(map.begin(), map.end(), frequentCompare);
}

bulgakov::node_t& bulgakov::getRareEl(map_t& map)
{
  return *std::min_element(map.begin(), map.end(), frequentCompare);
}

bool bulgakov::isNotSame(node_t& el, map_t& map)
{
  return map.find(el.first) == map.end();
}

bool bulgakov::isSame(node_t& el, map_t& map)
{
  return map.find(el.first) != map.end();
}
