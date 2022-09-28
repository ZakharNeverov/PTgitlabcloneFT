#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>
#include <stack>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <queue>
#include <set>
#include <map>
#include "graphAdditionalFunctions.hpp"

namespace mitkov {
  template< typename T, bool isOriented >
  class graph {
  private:
    std::vector< T > arr;
    std::vector< std::vector< bool > > matrix;

    void addVertex(const T& element);
    std::stack< int > searchCycle(int index, std::vector< short >& visited, int cameFrom, int& cycleIndex, bool& cycleFormed) const;

    graph< T, isOriented > searchSubgraph(
      const T& el,
      const std::set< T >& inter,
      std::map< T, bool >& visited,
      const graph< T, isOriented >& other,
      bool graphIsBeingGathered);

    void addConnection(int index1, int index2);

    void traverseDepth(std::vector< int >& result, std::vector< bool >& visited, int ind) const;

    void getSameSubgraphTraverseDepth(
      const graph< T, isOriented >& other,
      const std::set< T >& intersections,
      std::vector< bool >& checked,
      int elNumInFirst,
      graph< T, isOriented >& result) const;

    friend struct iterator;
  public:
    struct iterator {
      int index;
      graph < T, isOriented >* attachedGraph;
      std::vector< bool > visited = {};
      std::queue< int > nextSteps = {};
      
      iterator(int index, graph < T, isOriented >* attachedGraph):
        index(index),
        attachedGraph(attachedGraph)
      {
        if (index == -1) {
          return;
        }
        visited.resize(attachedGraph->arr.size());
        nextSteps.push(index);
        visited[index] = true;
      }
      
      bool operator==(const iterator& other) {
        return attachedGraph == other.attachedGraph && index == other.index;
      }

      bool operator!=(const iterator& other) {
        return !(*this == other);
      }

      iterator& operator++() {
        if (nextSteps.size() != 0) {
          do {
            index = nextSteps.front();
            nextSteps.pop();
          } while (visited[index] && nextSteps.size() > 0);
        }

        if (nextSteps.size() == 0) {
          size_t j;
          for (j = 0; j < visited.size() && visited[j]; ++j) {
            j = j + 1 - 1;
          }

          if (j == visited.size()) {
            *this = attachedGraph->end();
            return *this;
          }
          index = j;
        }

        visited[index] = true;
        for (size_t j = 0; j < attachedGraph->arr.size(); ++j) {
          if (attachedGraph->matrix[index][j]) {
            nextSteps.push(j);
          }
        }
        return *this;
      }

      T& operator*() {
        return attachedGraph->arr[index];
      }

      iterator& operator=(const iterator& other) {
        this->index = other.index;
        this->attachedGraph = other.attachedGraph;
        this->visited = other.visited;
        this->nextSteps = other.nextSteps;
        return *this;
      }
    };

    iterator begin() {
      return iterator(0, this);
    }

    iterator end() {
      return iterator(-1, nullptr);
    }

    int getSize();

    void add(const T& attachedTo, const T& element);
    void rAdd(const T& attachedTo, const T& element);
    void addFirstVertex(const T& element);

    void topologicSort();

    graph< T, isOriented > merge(const graph< T, isOriented >& other, const T& el1, const T& el2) const;

    graph< T, isOriented > getSameSubGraph(const graph< T, isOriented >& other) const;
    graph< T, isOriented > getCycle() const;
    int getMaxDegree() const;
    bool search(const T& key) const;
  };

  template< typename T >
  using orientedGraph = graph< T, true >;
  template< typename T >
  using undirGraph = graph< T, false >;


  template< typename T, bool isOriented >
  inline void graph< T, isOriented >::addVertex(const T& element) {
    for (auto& x: matrix) {
      x.push_back(false);
    }
    if (matrix.size() == 0) {
      matrix.push_back(std::vector< bool >(1));
    }
    else {
      matrix.push_back(std::vector< bool >(matrix[0].size()));
    }
    arr.push_back(element);
  }

  template< typename T, bool isOriented >
  inline std::stack< int > graph< T, isOriented >::searchCycle(int index, std::vector< short >& visited, int cameFrom, int& cycleIndex, bool& cycleFormed) const
  {
    if (visited[index] == 1) {
      cycleIndex = index;
      std::stack< int > result;
      result.push(index);
      return result;
    }
    if (visited[index] == 2) {
      return std::stack< int >();
    }
    visited[index] = 1;
    for (size_t i = 0; i < arr.size(); ++i) {
      if (matrix[index][i] && i != cameFrom) {
        std::stack< int > result = searchCycle(i, visited, index, cycleIndex, cycleFormed);
        if (cycleIndex == index || cycleFormed) {
          cycleFormed = true;
          return result;
        }
        if (cycleIndex != -1) {
          result.push(index);
          return result;
        }
      }
    }
    visited[index] = 2;
    return std::stack< int >();
  }

  template< typename T, bool isOriented >
  inline graph< T, isOriented > graph< T, isOriented >::searchSubgraph(
    const T& el,
    const std::set< T >& inter,
    std::map< T, bool >& visited,
    const graph< T, isOriented >& other,
    bool graphIsBeingGathered)
  {
    graph< T, isOriented > result{};
    int index1 = find(arr, el);
    int index2 = find(other.arr, el);
    for (int i = 0; i < arr.size(); ++i) {
      if (!matrix[index1][i] || inter.find(arr[i]) == inter.end()) {
        continue;
      }
      int a = find(other.arr, arr[i]);
      if (matrix[index2][a]) {
        if (result.arr.size() == 0) {
          result = searchSubgraph(arr[a], inter, visited, other, true);
          result.rAdd(*result.begin(), el);
        }
        else {
          graph< T, isOriented > temp = searchSubgraph(arr[a], inter, visited, other, true);
          result = result.merge(temp, *std::find(result.begin(), result.end(), el), *temp.begin());
        }
      }
    }
    if (result.arr.size() == 0 && graphIsBeingGathered) {
      result.addFirstVertex(el);
    }
    return result;
  }

  template< typename T, bool isOriented >
  inline void graph< T, isOriented >::addConnection(int index1, int index2) {
    matrix[index1][index2] = true;
    if (!isOriented) {
      matrix[index2][index1] = true;
    }
  }

  template< typename T, bool isOriented >
  inline void graph< T, isOriented >::traverseDepth(std::vector< int >& result, std::vector<bool>& visited, int ind) const {
    visited[ind] = true;
    for (size_t j = 0; j < matrix.size(); ++j) {
      if (matrix[ind][j] && !visited[j]) {
        traverseDepth(result, visited, j);
      }
    }
    result.push_back(ind);
  }

  template<typename T, bool isOriented>
  inline void graph< T, isOriented >::getSameSubgraphTraverseDepth(
    const graph< T, isOriented >& other,
    const std::set<T>& intersections,
    std::vector< bool >& checked,
    int elNumInFirst,
    graph< T, isOriented >& result) const
  {
    bool createdPoint = false;
    checked[elNumInFirst] = true;
    int elNumInSecond = find(other.arr, arr[elNumInFirst]);
    for (int i = 0; i < arr.size(); ++i) {
      if (matrix[elNumInFirst][i]) {
        if (checked[i]) {
          continue;
        }
        int num = find(other.arr, arr[i]);
        if (num != -1 && other.matrix[elNumInSecond][num]) {
          getSameSubgraphTraverseDepth(other, intersections, checked, i, result);
          if (!createdPoint) {
            createdPoint = true;
            result.rAdd(arr[i], arr[elNumInFirst]);
            continue;
          }
          result.addConnection(find(result.arr, arr[elNumInFirst]), find(result.arr, arr[i]));
        }
      }
    }
    if (result.arr.size() == 0) {
      result.addFirstVertex(arr[elNumInFirst]);
    }
  }

  template< typename T, bool isOriented >
  inline int graph< T, isOriented >::getSize() {
    return arr.size();
  }

  template< typename T, bool isOriented >
  inline void graph< T, isOriented >::add(const T& attachedTo, const T& element) {
    int indexA = mitkov::find(arr, attachedTo);
    int indexB = mitkov::find(arr, element);
    if (indexA == -1) {
      throw std::invalid_argument("Tried to attach to imaginary item");
    }
    if (indexB == -1) {
      addVertex(element);
      indexA = mitkov::find(arr, attachedTo);
      indexB = arr.size() - 1;
    }
    addConnection(indexA, indexB);
  }

  template< typename T, bool isOriented >
  inline void graph< T, isOriented >::rAdd(const T& attachedTo, const T& element) {
    int indexA = mitkov::find(arr, attachedTo);
    int indexB = mitkov::find(arr, element);
    if (indexA == -1) {
      throw std::invalid_argument("Tried to attach to imaginary item");
    }
    if (indexB == -1) {
      addVertex(element);
      indexA = mitkov::find(arr, attachedTo);
      indexB = arr.size() - 1;
    }
    addConnection(indexB, indexA);
  }

  template< typename T, bool isOriented >
  inline void graph< T, isOriented >::addFirstVertex(const T& element)
  {
    addVertex(element);
  }

  template< typename T, bool isOriented >
  inline void graph< T, isOriented >::topologicSort() {
    if (!isOriented) {
      throw std::invalid_argument("Unable to topologicSort disoriented Graph!");
    }
    std::vector< bool > isChecked(arr.size());
    std::vector< int > indResult;
    for (size_t i = 0; i < arr.size(); ++i) {
      if (isChecked[i]) {
        continue;
      }
      traverseDepth(indResult, isChecked, i);
    }

    size_t size = matrix.size() - 1;

    graph< T, isOriented > temp = *this;

    for (int i = 0; i < temp.arr.size(); ++i) {
      temp.arr[i] = arr[indResult[size - i]];
      temp.matrix[i] = matrix[indResult[size - i]];
    }

    for (int i = 0; i < matrix.size(); ++i) {
      for (int j = 0; j < matrix.size(); ++j) {
        temp.matrix[i][j] = matrix[indResult[size - i]][indResult[size - j]];
      }
    }

    *this = temp;
  }

  template< typename T, bool isOriented >
  inline graph< T, isOriented > graph< T, isOriented >::merge(const graph< T, isOriented >& other, const T& el1, const T& el2) const {
    graph< T, isOriented > result;
    result.arr.resize(arr.size(), other.arr.size());

    std::set< int > temp;
    for (auto& x: arr) {
      temp.insert(x);
    }
    for (auto& x: other.arr) {
      temp.insert(x);
    }
    std::copy(temp.begin(), temp.end(), result.arr.begin());

    
    result.matrix.resize(result.arr.size());
    for (auto& x: result.matrix) {
      x.resize(result.matrix.size());
    }
    for (size_t i = 0; i < arr.size(); ++i) {
      for (size_t j = 0; j < matrix[i].size(); ++j) {
        if (matrix[i][j]) {
          std::pair< int, int > pair = mitkov::findPair(result.arr, arr[i], arr[j]);
          result.addConnection(pair.first, pair.second);
        }
      }
    }
    for (size_t i = 0; i < other.arr.size(); ++i){
      for (size_t j = 0; j < other.matrix[i].size(); ++j) {
        if (other.matrix[i][j]) {
          std::pair< int, int > pair = mitkov::findPair(result.arr, other.arr[i], other.arr[j]);
          result.addConnection(pair.first, pair.second);
        }
      }
    }

    result.addConnection(find(result.arr, el1), find(result.arr, el2));
    return result;
  }

  template< typename T, bool isOriented >
  inline graph< T, isOriented > graph< T, isOriented >::getSameSubGraph(const graph< T, isOriented >& other) const
  {
    std::set< T > intersection;

    for (int i = 0; i < this->arr.size(); ++i) {
      intersection.insert(arr[i]);
    }
    for (int i = 0; i < other.arr.size(); ++i) {
      intersection.insert(other.arr[i]);
    }

    graph< T, isOriented > result;
    std::vector< bool > checked;
    checked.resize(arr.size());

    for (int i = 0; i < arr.size(); ++i) {
      if (intersection.find(arr[i]) != intersection.end()) {
        getSameSubgraphTraverseDepth(other, intersection, checked, arr[i], result);
        if (result.getSize() == 0) {
          result = graph< T, isOriented >();
        }
        else {
          return result;
        }
      }
    }
    return graph< T, isOriented >();
  }

  template< typename T, bool isOriented >
  inline graph< T, isOriented > graph< T, isOriented >::getCycle() const
  {
    std::vector< short > visited(arr.size());
    int cycleIndex = -1;
    bool cycleFormed = false;
    std::stack< int > indexResult = searchCycle(0, visited, 0, cycleIndex, cycleFormed);
    graph< T, isOriented > result;
    result.addFirstVertex(arr[indexResult.top()]);
    indexResult.pop();
    int j = 1;
    while (!indexResult.empty()) {
      result.addVertex(arr[indexResult.top()]);
      indexResult.pop();
      result.addConnection(j - 1, j);
      ++j;
    }
    result.addConnection(j - 1, 0);
    return result;
  }

  template< typename T, bool isOriented >
  inline int graph< T, isOriented >::getMaxDegree() const {
    int max = 0;
    for (auto& x: matrix) {
      int count = 0;
      for (auto y: x) {
        if (y) {
          ++count;
        }
      }
      if (count > max) {
        max = count;
      }
    }
    return max;
  }

  template< typename T, bool isOriented >
  inline bool graph< T, isOriented >::search(const T& key) const
  {
    return mitkov::find(arr, key) != -1;
  }
}

#endif
