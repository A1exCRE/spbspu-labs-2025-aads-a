#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "graphCollection.hpp"
#include "commands.hpp"

namespace
{
  void loadGraphsFromFile(std::ifstream & inputFile, smirnov::GraphCollection & graphs)
  {
    std::string graphName;
    size_t edgeCount;
    while (inputFile >> graphName >> edgeCount)
    {
      smirnov::Graph graph(graphName);
      for (size_t i = 0; i < edgeCount; ++i)
      {
        std::string from, to;
        unsigned weight;
        if (!(inputFile >> from >> to >> weight))
        {
          throw std::runtime_error("Input file format error");
        }
        graph.addEdge(from, to, weight);
      }
      if (!graphs.addGraph(graphName))
      {
        throw std::runtime_error("Duplicate graph name in input file: " + graphName);
      }
      smirnov::Graph * g = graphs.getGraph(graphName);
      if (g)
      {
        *g = graph;
      }
    }
  }
}

int main(int argc, char * argv[])
{
  if (argc < 2)
  {
    std::cerr << "Error: input filename required\n";
    return 1;
  }
  std::ifstream inputFile(argv[1]);
  if (!inputFile.is_open())
  {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }
  smirnov::GraphCollection graphs;
  try
  {
    loadGraphsFromFile(inputFile, graphs);
  }
  catch (const std::exception & ex)
  {
    std::cerr << "Error loading graphs: " << ex.what() << "\n";
    return 1;
  }
  smirnov::processCommands(graphs, std::cin, std::cout, std::cerr);
}
