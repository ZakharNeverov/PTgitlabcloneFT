#include "io_dicts.hpp"
#include <fstream>
#include <stdexcept>
#include "AdditionalStringMethods.hpp"

kurzov::dict_t kurzov::readDictFromStream(std::istream& in)
{
  dict_t dict;
  while (!in.eof())
  {
    std::string line = "";
    std::getline(in, line);
    if (!line.empty())
    {
      size_t space_index = line.find_first_of(' ');
      en_word_t en = line.substr(0, space_index);
      std::string ru_list_str = line.substr(space_index + 2, line.length() - space_index - 3);

      dict[en] = kurzov::splitStringToTranslation(ru_list_str);
    }
  }
  return dict;
}

kurzov::ru_list_t kurzov::splitStringToTranslation(std::string str)
{
  ru_list_t ru_list;
  while (!str.empty())
  {
    ru_list.push_back(kurzov::getNextWord(str, ','));
  }
  return ru_list;
}

void kurzov::printDict(const dict_t& dict, std::ostream& out, const char* delimeter, bool with_new_line)
{
  bool is_first = true;
  for (auto&& en_ru: dict)
  {
    if (!is_first)
    {
      out << '\n';
    }
    is_first = false;
    out << en_ru.first << ' ';
    printRuList(en_ru.second, out, delimeter, false);
  }
  if (with_new_line)
  {
    out << '\n';
  }
}

void kurzov::printRuList(const ru_list_t& list, std::ostream& out, const char* delimeter, bool with_new_line)
{
  out << '[';
  bool is_first = true;
  for (auto&& ru: list)
  {
    if (!is_first)
    {
      out << delimeter;
    }
    is_first = false;
    out << ru;
  }
  out << ']';
  if (with_new_line)
  {
    out << '\n';
  }
}

bool kurzov::loadNewDict(std::string dict_name, std::string filename, dicts_t& dicts)
{
  std::ifstream fin(filename);
  if (!fin.is_open())
  {
    return false;
  }

  dicts[dict_name] = kurzov::readDictFromStream(fin);

  fin.close();

  return true;
}
