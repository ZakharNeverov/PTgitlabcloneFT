#include "commands.hpp"
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <functional>
#include <vector>
#include "messagesIO.hpp"

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

  size_t getTotalCount(const bokiy::map_t& map)
  {
    size_t totalCount = 0;
    for (auto&& word: map)
    {
      totalCount += word.second;
    }
    return totalCount;
  }

  bool isMapExist(std::string& mapName, bokiy::maps_t& maps)
  {
    return maps.find(mapName) != maps.end();
  }
}

void bokiy::make(std::istream& istr, maps_t& maps)
{
  std::vector< std::string > words = getWords(istr);
  if (words.size() != 2)
  {
    throw std::logic_error("Invalid command.");
  }
  std::string fileName = words[1];
  std::fstream in;
  in.open(fileName);
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

void bokiy::write(std::istream& istr, maps_t& maps)
{
  std::vector< std::string > words = getWords(istr);
  if (words.size() != 2)
  {
    throw std::logic_error("Invalid command.");
  }
  std::string fileName = words[0];
  std::fstream in;
  in.open(fileName);
  if (!in.is_open())
  {
    throw std::logic_error("Error, file is not open.");
  }
  std::string mapName = words[1];
  if (!isMapExist(mapName, maps))
  {
    throw std::logic_error("Error, map is not exist.");
  }
  map_t map = maps[mapName];
  for (auto word : map)
  {
    in << word.first << " " << word.second << '\n';
  }
}

void bokiy::read(std::istream& istr, maps_t& maps)
{
  std::vector< std::string > words = getWords(istr);
  if (words.size() != 2)
  {
    throw std::logic_error("Invalid command.");
  }
  std::string fileName = words[1];
  std::fstream in;
  in.open(fileName);
  if (!in.is_open())
  {
    throw std::logic_error("Error, file is not open.");
  }
  map_t map;
  std::string key = "";
  std::string value = "";
  while (!in.eof())
  {
    in >> key;
    in >> value;
    
    map.insert({key, std::stoi(value)});
  }
  std::string mapName = words[0];
  maps[mapName] = map;
}

void bokiy::makeFrequentOrRarest(std::istream& istr, maps_t& maps, get_el_f_t_ getEl)
{
  std::vector< std::string > words = getWords(istr);
  if (words.size() != 3)
  {
    throw std::logic_error("Invalid command.");
  }
  std::string baseMapName = words[1];
  if (!isMapExist(baseMapName, maps))
  {
    throw std::logic_error("Error, map is not exist.");
  }
  double coef = std::stod(words[2]);
  if (coef < 0 || coef > 1)
  {
    throw std::logic_error("Invalid command.");
  }
  map_t baseMap = maps[baseMapName];
  size_t countFrequentWords = coef * baseMap.size();
  map_t temp = baseMap;
  map_t newMap;
  for (size_t i = 0; i < countFrequentWords; i++)
  {
    bokiy::node_t frEl = getEl(temp);
    newMap.insert(frEl);
    temp.erase(frEl.first);
  }
  std::string newMapName = words[0];
  maps[newMapName] = newMap;
}

void bokiy::complementOrIntersect(std::istream& istr, maps_t& maps, comp_f_t comp)
{
  std::vector< std::string > words = getWords(istr);
  if (words.size() != 3)
  {
    throw std::logic_error("Invalid command.");
  }
  std::string firstName = words[1];
  std::string secondName = words[2];
  if (!isMapExist(firstName, maps) || !isMapExist(secondName, maps))
  {
    throw std::logic_error("Error, map is not exist.");
  }
  map_t firstMap = maps[firstName];
  map_t secondMap = maps[secondName];
  map_t newMap;
  using namespace std::placeholders;
  std::copy_if(firstMap.begin(), firstMap.end(), std::inserter(newMap, newMap.end()), std::bind(comp, _1, secondMap));
  std::string newMapName = words[0];
  maps[newMapName] = newMap;
}

void bokiy::print(std::istream& istr, maps_t& maps)
{
  std::vector< std::string > words = getWords(istr);
  if (words.size() != 1)
  {
    throw std::logic_error("Invalid command.");
  }
  std::string mapName = words[0];
  if (!isMapExist(mapName, maps))
  {
    throw std::logic_error("Error, map is not exist.");
  }
  map_t map = maps[mapName];
  size_t totalCount = getTotalCount(map);
  std::cout << std::fixed << std::setprecision(2);
  for (auto word : map)
  {
    std::cout << word.first << " " << static_cast<double>(word.second) / totalCount << '\n';
  }
}

void bokiy::printFrequentOrRarestWord(std::istream& istr, maps_t& maps, get_el_f_t_ getEl)
{
  std::vector< std::string > words = getWords(istr);
  if (words.size() != 1)
  {
    throw std::logic_error("Invalid command.");
  }
  std::string mapName = words[0];
  if (!isMapExist(mapName, maps))
  {
    throw std::logic_error("Error, map is not exist.");
  }
  map_t map = maps[mapName];
  std::cout << getEl(map).first << '\n';
}

bool bokiy::frequentCompare(node_t& first, node_t& second)
{
  return first.second < second.second;
}

bokiy::node_t& bokiy::getFrequentElement(map_t& map)
{
  return *std::max_element(map.begin(), map.end(), frequentCompare);
}

bokiy::node_t& bokiy::getRarestElement(map_t& map)
{
  return *std::min_element(map.begin(), map.end(), frequentCompare);
}

bool bokiy::isNotSame(node_t& el, map_t& map)
{
  return map.find(el.first) == map.end();
}

bool bokiy::isSame(node_t& el, map_t& map)
{
  return map.find(el.first) != map.end();
}
