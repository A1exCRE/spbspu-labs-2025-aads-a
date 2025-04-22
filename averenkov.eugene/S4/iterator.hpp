#ifndef TREE_ITERATOR_HPP
#define TREE_ITERATOR_HPP

#include "node.hpp"
#include <iterator>
#include <cassert>

template < class Key, class Value, class Compare >
class Iterator : public std::iterator< std::bidirectional_iterator_tag, std::pair< const Key, Value > >
{
public:
  Iterator();
  explicit Iterator(Node< Key, Value >* node);

  std::pair< const Key, Value >& operator*() const;
  std::pair< const Key, Value >* operator->() const;

  Iterator& operator++();
  Iterator operator++(int);

  Iterator& operator--();
  Iterator operator--(int);

  bool operator==(const Iterator& other) const;
  bool operator!=(const Iterator& other) const;

private:
  Node< Key, Value >* current;

};

template < class Key, class Value, class Compare >
Iterator< Key, Value, Compare >::Iterator():
  current(nullptr)
{
}

template < class Key, class Value, class Compare >
Iterator< Key, Value, Compare >::Iterator(Node< Key, Value >* node):
  current(node)
{
}

template < class Key, class Value, class Compare >
std::pair< const Key, Value >& Iterator< Key, Value, Compare >::operator*() const
{
  assert(current != nullptr);
  return current->data;
}

template < class Key, class Value, class Compare >
std::pair< const Key, Value >* Iterator< Key, Value, Compare >::operator->() const
{
  assert(current != nullptr);
  return std::addressof(current->data);
}

template < class Key, class Value, class Compare >
Iterator< Key, Value, Compare >&
Iterator< Key, Value, Compare >::operator++()
{
  assert(current != nullptr);
  if (current->right)
  {
    current = current->right;
    while (current->left)
    {
      current = current->left;
    }
  }
  else
  {
    NodeType* parent = current->parent;
    while (parent && current == parent->right)
    {
      current = parent;
      parent = parent->parent;
    }
    current = (parent && parent != fake_root) ? parent : nullptr;
  }
  return *this;
}

template < class Key, class Value, class Compare >
Iterator< Key, Value, Compare >
Iterator< Key, Value, Compare >::operator++(int)
{
  Iterator tmp = *this;
  ++(*this);
  return tmp;
}

template < class Key, class Value, class Compare >
Iterator< Key, Value, Compare >&
Iterator< Key, Value, Compare >::operator--()
{
  assert(current != nullptr);
  if (current->left)
  {
    current = current->left;
    while (current->right)
    {
      current = current->right;
    }
  }
  else
  {
    NodeType* parent = current->parent;
    while (parent && current == parent->left)
    {
      current = parent;
      parent = parent->parent;
    }
    current = (parent && parent != fake_root) ? parent : nullptr;
  }
  return *this;
}

template < class Key, class Value, class Compare >
Iterator< Key, Value, Compare >
Iterator< Key, Value, Compare >::operator--(int)
{
  Iterator tmp = *this;
  --(*this);
  return tmp;
}

template < class Key, class Value, class Compare >
bool Iterator< Key, Value, Compare >::operator==(const Iterator& other) const
{
  return current == other.current;
}

template < class Key, class Value, class Compare >
bool Iterator< Key, Value, Compare >::operator!=(const Iterator& other) const
{
  return current != other.current;
}

#endif
