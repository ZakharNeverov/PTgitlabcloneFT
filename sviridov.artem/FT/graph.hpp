#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>
#include <iosfwd>

namespace sviridov
{
  class MatrixGraph
  {
  public:
    explicit MatrixGraph(std::vector< std::vector< int > >& matrixGraph);
    bool hasVertex(size_t vertex);
    bool hasArc(size_t vertexFrom, size_t vertexTo);
    void removeArc(size_t vertexFrom, size_t vertexTo);
    std::vector< int > inVertexesDegrees();
    std::vector< int > outVertexesDegrees();
    size_t findVertexWithMaxDegree();
    void removeMultipleArcs();
    std::vector< size_t > findSourceVertexes();
    std::vector< size_t > findSinkVertexes();
    std::vector< size_t > findAdjecentVertexes(size_t vertex);
    std::vector< size_t > findPath(size_t from, size_t to);
    void removeCycles(bool onlyHamiltonian);
    size_t getMatrixSize();
    const std::vector< std::vector< int > >& getData();
  private:
    std::vector< std::vector< int > > matrixGraph_;
    int inVertexDegree(size_t vertex);
    int outVertexDegree(size_t vertex);
    void doRemovingCycles(size_t row, std::vector< std::string >& colors);
    void doRemovingHamiltonianCycles(size_t row, std::vector< std::string >& colors);
    void doFindingPath(size_t from, size_t to, std::vector< size_t >& out, std::vector< std::string >& colors);
  };
}

#endif
