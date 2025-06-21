#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <string>
#include "hashTable.hpp"

namespace smirnov
{
  class Graph
  {
  public:
    Graph() = default;
    Graph(const std::string & name);
    const std::string & getName() const noexcept;
    bool hasVertex(const std::string & vertex) const;
    bool addVertex(const std::string & vertex);
    bool addEdge(const std::string & from, const std::string & to, unsigned weight);
    bool removeEdge(const std::string & from, const std::string & to, unsigned weight);
    std::vector< std::string > getVertices() const;
    std::vector< std::pair< std::string, unsigned > > getOutboundEdges(const std::string & vertex) const;
    std::vector< std::pair< std::string, unsigned > > getInboundEdges(const std::string & vertex) const;
  private:
    std::string name_;
    HashTable< std::string, HashTable< std::string, std::vector< unsigned > > > edges_;
  };
}
#endif
