#include "graph.hpp"
#include <algorithm>

smirnov::Graph::Graph(const std::string & name):
  name_(name)
{}

const std::string & smirnov::Graph::getName() const noexcept
{
  return name_;
}

bool smirnov::Graph::hasVertex(const std::string & vertex) const
{
  return edges_.find(vertex) != edges_.end();
}

bool smirnov::Graph::addVertex(const std::string & vertex)
{
  if (edges_.find(vertex) != edges_.end())
  {
    return false;
  }
  edges_.insert(vertex, smirnov::HashTable< std::string, std::vector< unsigned > >());
  return true;
}

bool smirnov::Graph::addEdge(const std::string & from, const std::string & to, unsigned weight)
{
  addVertex(from);
  addVertex(to);
  auto fromIt = edges_.find(from);
  if (fromIt == edges_.end())
  {
    return false;
  }
  auto & toTable = const_cast< HashTable< std::string, std::vector< unsigned > > & >(fromIt->second);
  auto toIt = toTable.find(to);
  if (toIt == toTable.end())
  {
    std::vector< unsigned > weights;
    toTable.insert(to, weights);
  }
  else
  {
    auto & weights = const_cast< std::vector< unsigned > & >(toIt->second);
    weights.push_back(weight);
  }
  return true;
}

bool smirnov::Graph::removeEdge(const std::string & from, const std::string & to, unsigned weight)
{
  auto fromIt = edges_.find(from);
  if (fromIt == edges_.end())
  {
    return false;
  }
  auto & toTable = const_cast< smirnov::HashTable< std::string, std::vector< unsigned > > & >(fromIt->second);
  auto toIt = toTable.find(to);
  if (toIt == toTable.end())
  {
    return false;
  }
  auto & weights = const_cast< std::vector< unsigned > & >(toIt->second);
  for (size_t i = 0; i < weights.size(); ++i)
  {
    if (weights[i] == weight)
    {
      weights.erase(weights.begin() + i);
      if (weights.empty())
      {
        toTable.erase(to);
      }
      return true;
    }
  }
  return false;
}

std::vector< std::string > smirnov::Graph::getVertices() const
{
  std::vector< std::string > vertices;
  for (auto it = edges_.begin(); it != edges_.end(); ++it)
  {
    vertices.push_back(it->first);
  }
  std::sort(vertices.begin(), vertices.end());
  return vertices;
}

std::vector< std::pair< std::string, unsigned > > smirnov::Graph::getOutboundEdges(const std::string & vertex) const
{
  std::vector< std::pair< std::string, unsigned > > result;
  auto vertexIt = edges_.find(vertex);
  if (vertexIt == edges_.end())
  {
    return result;
  }
  const auto & outEdges = vertexIt->second;
  for (auto it = outEdges.begin(); it != outEdges.end(); ++it)
  {
    const std::string & target = it->first;
    const std::vector< unsigned > & weights = it->second;
    for (size_t i = 0; i < weights.size(); ++i)
    {
      result.push_back(std::make_pair(target, weights[i]));
    }
  }
  for (size_t i = 0; i < result.size(); ++i)
  {
    for (size_t j = i + 1; j < result.size(); ++j)
    {
      if (result[i].first > result[j].first || (result[i].first == result[j].first && result[i].second > result[j].second))
      {
        std::swap(result[i], result[j]);
      }
    }
  }
  return result;
}

std::vector< std::pair< std::string, unsigned > > smirnov::Graph::getInboundEdges(const std::string & vertex) const
{
  std::vector< std::pair< std::string, unsigned > > result;
  for (auto fromIt = edges_.begin(); fromIt != edges_.end(); ++fromIt)
  {
    const std::string & source = fromIt->first;
    const auto & outEdges = fromIt->second;
    auto toIt = outEdges.find(vertex);
    if (toIt != outEdges.end())
    {
      const std::vector< unsigned > & weights = toIt->second;
      for (size_t i = 0; i < weights.size(); ++i)
      {
        result.push_back(std::make_pair(source, weights[i]));
      }
    }
  }
  for (size_t i = 0; i < result.size(); ++i)
  {
    for (size_t j = i + 1; j < result.size(); ++j)
    {
      if (result[i].first > result[j].first || (result[i].first == result[j].first && result[i].second > result[j].second))
      {
        std::swap(result[i], result[j]);
      }
    }
  }
  return result;
}
