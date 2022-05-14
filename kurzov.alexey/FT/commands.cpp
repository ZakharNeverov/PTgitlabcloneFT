#include "commands.hpp"
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <functional>
#include "io_structs.hpp"
#include "StreamGuard.hpp"
#include "processing_dicts.hpp"
#include "CommandsMessages.hpp"
#include "AdditionalStringMethods.hpp"

namespace
{
  void prepareString(std::string& str)
  {
    size_t first_not_whitespace = str.find_first_not_of(' ');
    str.erase(0, first_not_whitespace);
  }
  std::vector< std::string > getNamesFromString(std::string str)
  {
    prepareString(str);
    std::vector< std::string > names;
    while (!str.empty())
    {
      names.push_back(kurzov::getNextWord(str));
    }
    return names;
  }
  bool isValidDict(const std::string& name, const kurzov::dicts_t& dicts)
  {
    return dicts.find(name) != dicts.end();
  }
  bool isValidDicts(const std::vector< std::string >& names, const kurzov::dicts_t& dicts)
  {
    using namespace std::placeholders;
    return std::all_of(names.begin(), names.end(), std::bind(isValidDict, _1, std::ref(dicts)));
  }
}

void kurzov::doPrint(const std::string& str, dicts_t& dicts, std::ostream& out)
{
  std::vector< std::string > names = getNamesFromString(str);
  if (names.size() != 1 || !isValidDicts(names, dicts))
  {
    throw std::invalid_argument("Bad dict names!");
  }
  dicts_t::iterator dict_to_print_iter = dicts.find(names.at(0));

  const dict_t& dict_to_print = dict_to_print_iter->second;
  if (dict_to_print.empty())
  {
    kurzov::outEmpty(out);
  }
  else
  {
    kurzov::printDict(dict_to_print, out);
  }
}

void kurzov::doUnion(const std::string& str, dicts_t& dicts)
{
  std::vector< std::string > names = getNamesFromString(str);
  if (names.size() < 3)
  {
    throw std::invalid_argument("Bad dicts number!");
  }
  auto names_iter = names.begin();
  std::string new_dict_name = *names_iter;
  names_iter = names.erase(names_iter);
  if (!isValidDicts(names, dicts))
  {
    throw std::invalid_argument("Bad dict names!");
  }

  dicts_t::iterator first_dict_iter = dicts.find(*(names_iter++));
  dicts_t::iterator second_dict_iter = dicts.find(*(names_iter++));

  dict_t new_dict = kurzov::unionDicts(first_dict_iter->second, second_dict_iter->second);
  while (names_iter != names.end())
  {
    second_dict_iter = dicts.find(*(names_iter++));
    kurzov::unionDict(second_dict_iter->second, new_dict);
  }

  dicts[new_dict_name] = new_dict;
}

void kurzov::doComplement(const std::string& str, dicts_t& dicts)
{
  std::vector< std::string > names = getNamesFromString(str);
  if (names.size() != 3)
  {
    throw std::invalid_argument("Bad dicts number!");
  }
  auto names_iter = names.begin();
  std::string new_dict_name = *names_iter;
  names_iter = names.erase(names_iter);
  if (!isValidDicts(names, dicts))
  {
    throw std::invalid_argument("Bad dict names!");
  }

  dicts_t::iterator first_dict_iter = dicts.find(*(names_iter++));
  dicts_t::iterator second_dict_iter = dicts.find(*(names_iter++));

  dict_t first_dict = first_dict_iter->second;
  dict_t second_dict = second_dict_iter->second;

  dict_t new_dict = kurzov::comlementDicts(first_dict, second_dict);

  dicts[new_dict_name] = new_dict;
}

void kurzov::doIntersect(const std::string& str, dicts_t& dicts)
{
  std::vector< std::string > names = getNamesFromString(str);
  if (names.size() < 3)
  {
    throw std::invalid_argument("Bad dicts number!");
  }
  auto names_iter = names.begin();
  std::string new_dict_name = *names_iter;
  names_iter = names.erase(names_iter);
  if (!isValidDicts(names, dicts))
  {
    throw std::invalid_argument("Bad dict names!");
  }

  dicts_t::iterator first_dict_iter = dicts.find(*(names_iter++));
  dicts_t::iterator second_dict_iter = dicts.find(*(names_iter++));

  dict_t new_dict = kurzov::intersectDicts(first_dict_iter->second, second_dict_iter->second);
  while (names_iter != names.end())
  {
    second_dict_iter = dicts.find(*(names_iter++));
    new_dict = kurzov::intersectDicts(second_dict_iter->second, new_dict);
  }

  dicts[new_dict_name] = new_dict;
}

void kurzov::doLoad(const std::string& str, dicts_t& dicts)
{
  std::vector< std::string > names = getNamesFromString(str);
  if (names.size() != 2)
  {
    throw std::invalid_argument("Bad number of args!");
  }
  std::string new_dict_name = names.at(0);
  std::string new_dict_filename = names.at(1);

  if (!loadNewDict(new_dict_name, new_dict_filename, dicts))
  {
    throw std::logic_error("Bad dict!");
  }
}

void kurzov::doTranslate(const std::string& str, dicts_t& dicts, std::ostream& out)
{
  std::vector< std::string > names = getNamesFromString(str);
  if (names.size() != 2)
  {
    throw std::invalid_argument("Bad number of args!");
  }

  if (isValidDict(names.at(0), dicts))
  {
    const dict_t& dict = dicts[names.at(0)];
    std::string english_word = names.at(1);

    auto translation_iter = dict.find(english_word);
    if (translation_iter == dict.end())
    {
      kurzov::outNone(out);
    }
    else
    {
      out << english_word << ' ';
      kurzov::printRuList(translation_iter->second, out);
    }
  }
  else
  {
    throw std::logic_error("Bad args!");
  }
}
