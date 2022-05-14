#include "processing_dicts.hpp"

kurzov::dict_t kurzov::concatDicts(const dict_t& first, const dict_t& second)
{
  dict_t new_dict = first;
  for (auto&& second_data: second)
  {
    bool is_uniq = true;
    for (auto& new_dict_data: new_dict)
    {
      if (second_data.first == new_dict_data.first)
      {
        is_uniq = false;
        new_dict_data.second = concatRuLists(second_data.second, new_dict_data.second);
        break;
      }
    }
    if (is_uniq)
    {
      new_dict[second_data.first] = second_data.second;
    }
  }
  return new_dict;
}

kurzov::ru_list_t kurzov::concatRuLists(const ru_list_t& first, const ru_list_t& second)
{
  ru_list_t new_list = first;
  for (auto&& second_data: second)
  {
    new_list.push_back(second_data);
  }
  for (auto&& first_data: first)
  {
    new_list.push_back(first_data);
  }
  return new_list;
}

kurzov::dict_t kurzov::unionDicts(const dict_t& first, const dict_t& second)
{
  dict_t new_dict;
  unionDict(first, new_dict);
  unionDict(second, new_dict);
  return new_dict;
}

void kurzov::unionDict(const dict_t& dict, dict_t& dest)
{
  for (auto&& dict_data: dict)
  {
    bool is_uniq = true;
    for (auto&& dest_data: dest)
    {
      if (dest_data.first == dict_data.first)
      {
        dest_data.second = {};
        is_uniq = false;
        break;
      }
    }
    if (is_uniq)
    {
      dest[dict_data.first] = dict_data.second;
    }
  }
}

kurzov::dict_t kurzov::intersectDicts(const dict_t& first, const dict_t& second)
{
  dict_t new_dict;
  for (auto&& first_data: first)
  {
    for (auto&& second_data: second)
    {
      if (first_data.first == second_data.first)
      {
        new_dict[first_data.first] = intersectRuList(first_data.second, second_data.second);
      }
    }
  }
  return new_dict;
}

kurzov::ru_list_t kurzov::intersectRuList(const ru_list_t& first, const ru_list_t& second)
{
  ru_list_t new_list;
  for (auto&& first_data: first)
  {
    for (auto&& second_data: second)
    {
      if (first_data == second_data)
      {
        new_list.push_back(first_data);
        break;
      }
    }
  }
  return new_list;
}

kurzov::dict_t kurzov::comlementDicts(const dict_t& first, const dict_t& second)
{
  dict_t new_dict = first;

  for (auto&& second_data: second)
  {
    for (auto& new_dict_data: new_dict)
    {
      if (second_data.first == new_dict_data.first)
      {
        new_dict_data.second = comlementRuLists(new_dict_data.second, second_data.second);
        break;
      }
    }
  }

  return new_dict;
}

kurzov::ru_list_t kurzov::comlementRuLists(const ru_list_t& first, const ru_list_t& second)
{
  ru_list_t new_list;

  for (auto&& first_data: first)
  {
    bool is_uniq = true;
    for (auto&& second_data: second)
    {
      if (first_data == second_data)
      {
        is_uniq = false;
        break;
      }
    }
    if (is_uniq)
    {
      new_list.push_back(first_data);
    }
  }

  return new_list;
}
