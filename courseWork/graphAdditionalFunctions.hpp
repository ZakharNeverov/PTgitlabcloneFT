#ifndef GRAPH_ADDITIONAL_FUNCTIONS_HPP
#define GRAPH_ADDITIONAL_FUNCTIONS_HPP
#include <vector>

namespace mitkov {
  template< typename T >
  int find(const std::vector< T >& arr, const T& key) {
    for (size_t i = 0; i < arr.size(); ++i) {
      if (arr[i] == key) {
        return i;
      }
    }
    return -1;
  }

  template< typename T >
  std::pair< int, int > findPair(const std::vector< T >& arr, const T& key1, const T& key2) {
    int el1 = -1;
    int el2 = -1;
    for (size_t i = 0; i < arr.size(); ++i) {
      if (arr[i] == key1) {
        el1 = i;
      }
      if (arr[i] == key2) {
        el2 = i;
      }
      if (el1 != -1 && el2 != -1) {
        return std::pair< int, int >(el1, el2);
      }
    }
    return std::pair< int, int >(-1, -1);
  }
}

#endif
