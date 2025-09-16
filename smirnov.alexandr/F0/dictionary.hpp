#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <vector>
#include <string>
#include <hash_table/hashTable.hpp>

namespace smirnov
{
  using Dict = HashTable< std::string, std::vector< std::string > >;
  using Dicts = HashTable< std::string, Dict >;
}
#endif
