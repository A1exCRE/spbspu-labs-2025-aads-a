#include "list-manip.hpp"

size_t sveshnikov::getSizeDataList(iter_t i)
{
  size_t len = 0;
  for (iter_list_ull_t j = i->second.cbegin(); j != i->second.cend(); j++)
  {
    len++;
  }
  return len;
}

size_t sveshnikov::getNumNewLists(const list_pair_t &list)
{
  size_t num_lines = 0;
  for (iter_t i = list.cbegin(); i != list.cend(); i++)
  {
    num_lines = std::max(num_lines, getSizeDataList(i));
  }
  return num_lines;
}
