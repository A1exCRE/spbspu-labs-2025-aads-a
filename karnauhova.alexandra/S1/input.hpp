#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <string>
#include <forward_list>

namespace karnauhova
{
  std::pair<std::string, std::forward_list<unsigned long long>> input_line(std::istream& in, std::string name);
}

#endif
