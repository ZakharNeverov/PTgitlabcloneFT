#include "processing_dicts.hpp"

#include <functional>
#include <algorithm>

namespace
{
  template < typename Compare >
  struct KeysCmp
  {
    template < typename T >
    bool operator()(const T& a, const T& b)
    {
      return Compare()(a.first, b.first);
    }
  };
  template < typename T >
  using KeysEqual = KeysCmp< std::equal_to< T > >;
}

kurzov::dict_t kurzov::concatDicts(const dict_t& first, const dict_t& second)
{
  dict_t new_dict;
  auto cmp_less = KeysCmp< dict_t::key_compare >{};
  std::set_union(first.begin(), first.end(), second.begin(), second.end(), std::inserter(new_dict, new_dict.end()), cmp_less);

  dict_t inters;
  std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::inserter(inters, inters.end()), cmp_less);

  std::for_each(new_dict.begin(), new_dict.end(),
   [&first,&second](dict_t::value_type& pair)
   {
     pair.second = concatRuLists(first.at(pair.first), second.at(pair.first));
   });

  return new_dict;
}

kurzov::ru_list_t kurzov::concatRuLists(const ru_list_t& first, const ru_list_t& second)
{
  ru_list_t new_list = first;
  std::copy(first.begin(), first.end(), std::inserter(new_list, new_list.end()));
  std::copy(second.begin(), second.end(), std::inserter(new_list, new_list.end()));
  return new_list;
}

kurzov::dict_t kurzov::unionDicts(const dict_t& first, const dict_t& second)
{
  dict_t new_dict;
  auto cmp_eq = KeysEqual< en_word_t >{};
  auto cmp_less = KeysCmp< dict_t::key_compare >{};
  std::set_union(first.begin(), first.end(), second.begin(), second.end(), std::inserter(new_dict, new_dict.end()), cmp_less);

  dict_t inters;
  std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::inserter(inters, inters.end()), cmp_less);

  std::for_each(new_dict.begin(), new_dict.end(),
   [&inters,&cmp_eq](dict_t::value_type& pair)
   {
     if (std::count_if(inters.begin(), inters.end(), std::bind(cmp_eq, std::placeholders::_1, pair)))
     {
       pair.second = {};
     }
   });
  return new_dict;
}

kurzov::dict_t kurzov::intersectDicts(const dict_t& first, const dict_t& second)
{
  dict_t new_dict;
  auto cmp_less = KeysCmp< dict_t::key_compare >{};
  std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::inserter(new_dict, new_dict.end()), cmp_less);

  std::for_each(new_dict.begin(), new_dict.end(),
   [&second](dict_t::value_type& pair)
   {
     pair.second = intersectRuList(pair.second, second.at(pair.first));
   });

  return new_dict;
}

kurzov::ru_list_t kurzov::intersectRuList(const ru_list_t& first, const ru_list_t& second)
{
  ru_list_t new_list;
  std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::inserter(new_list, new_list.end()));
  return new_list;
}

kurzov::dict_t kurzov::complementDicts(const dict_t& first, const dict_t& second)
{
  dict_t new_dict = first;
  auto cmp_less = KeysCmp< dict_t::key_compare >{};

  dict_t inters;
  std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::inserter(inters, inters.end()), cmp_less);

  std::for_each(inters.begin(), inters.end(),
   [&new_dict,&first,&second](const dict_t::value_type& pair)
   {
     new_dict[pair.first] = complementRuLists(first.at(pair.first), second.at(pair.first));
   });

  return new_dict;
}

kurzov::ru_list_t kurzov::complementRuLists(const ru_list_t& first, const ru_list_t& second)
{
  ru_list_t new_list;
  std::set_difference(first.begin(), first.end(), second.begin(), second.end(), std::inserter(new_list, new_list.begin()));
  return new_list;
}
