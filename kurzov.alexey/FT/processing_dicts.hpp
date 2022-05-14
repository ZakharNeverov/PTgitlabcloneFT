#ifndef PROCESSING_DICTS_HPP
#define PROCESSING_DICTS_HPP

#include "io_dicts.hpp"

namespace kurzov
{
  dict_t concatDicts(const dict_t&, const dict_t&);
  ru_list_t concatRuLists(const ru_list_t&, const ru_list_t&);

  dict_t unionDicts(const dict_t&, const dict_t&);

  dict_t intersectDicts(const dict_t&, const dict_t&);
  ru_list_t intersectRuList(const ru_list_t&, const ru_list_t&);

  dict_t comlementDicts(const dict_t&, const dict_t&);
  ru_list_t comlementRuLists(const ru_list_t&, const ru_list_t&);
}

#endif
