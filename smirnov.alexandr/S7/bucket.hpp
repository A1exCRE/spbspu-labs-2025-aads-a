#ifndef BUCKET_HPP
#define BUCKET_HPP
#include <utility>

namespace smirnov
{
  template< class Key, class Value >
  struct Bucket
  {
    std::pair< Key, Value > data;
    bool occupied = false;
    Bucket():
      data(),
      occupied(false)
    {}
    Bucket(const Key & k, const Value & v):
      data(k ,v),
      occupied(true)
    {}
  };
}
#endif
