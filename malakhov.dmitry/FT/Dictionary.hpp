#ifndef DICT_HPP
#define DICT_HPP

#include "AVLTree.hpp"

namespace malakhov
{
  template < typename KeyT, typename ValueT, typename ComparatorT = std::less< KeyT > >
  using Dictionary = detail::AVLTree< KeyT, ValueT, ComparatorT >;
}

#endif
