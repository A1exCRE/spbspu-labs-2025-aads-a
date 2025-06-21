#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include "graphCollection.hpp"

namespace smirnov
{
  void printError(std::ostream & err);
  void processCommands(GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err);
  void command_graphs(const GraphCollection & graphs, std::ostream & out);
  void vertexes(const GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err);
  void outbound(const GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err);
  void inbound(const GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err);
  void bind(GraphCollection & graphs, std::istream & in, std::ostream & err);
  void cut(GraphCollection & graphs, std::istream & in, std::ostream & err);
  void create(GraphCollection & graphs, std::istream & in, std::ostream & err);
  void merge(GraphCollection & graphs, std::istream & in, std::ostream & err);
  void extract(GraphCollection & graphs, std::istream & in, std::ostream & err);
}
#endif
