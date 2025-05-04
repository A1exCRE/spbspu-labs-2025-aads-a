#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "node.hpp"
#include "iterator.hpp"
#include "constIterator.hpp"
#include <utility>
#include <initializer_list>
#include <stdexcept>
#include <stack>
#include <unordered_map>

template < class Key, class Value, class Compare = std::less< Key > >
class Tree
{
public:
  using NodeType = Node< Key, Value >;
  using iterator = Iterator< Key, Value, Compare >;
  using const_iterator = ConstIterator< Key, Value, Compare >;

  Tree();
  explicit Tree(const Compare& cmp);
  Tree(const Tree& other);
  Tree(Tree&& other) noexcept;
  Tree(std::initializer_list< std::pair< const Key, Value > > init, const Compare& cmp);
  template< class InputIt >
  Tree(InputIt first, InputIt last, const Compare& cmp);
  ~Tree();

  Tree& operator=(const Tree& other);
  Tree& operator=(Tree&& other) noexcept;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

  Value& at(const Key& key);
  const Value& at(const Key& key) const;
  Value& operator[](const Key& key);
  const Value& operator[](const Key& key) const;

  iterator insert(const std::pair< const Key, Value >& value);
  void push(const Key& key, const Value& value);
  template < class InputIt >
  void insert(InputIt first, InputIt last);
  void insert(std::initializer_list< std::pair< const Key, Value > > ilist);

  iterator erase(iterator pos);
  iterator erase(iterator first, iterator last);
  size_t erase(const Key& key);
  void clear();
  void swap(Tree& other) noexcept;

  size_t count(const Key& key) const;
  iterator find(const Key& key);
  const_iterator find(const Key& key) const;
  std::pair< iterator, iterator > equal_range(const Key& key);
  std::pair< const_iterator, const_iterator > equal_range(const Key& key) const;
  iterator lower_bound(const Key& key);
  const_iterator lower_bound(const Key& key) const;
  iterator upper_bound(const Key& key);
  const_iterator upper_bound(const Key& key) const;

  bool empty() const;
  size_t size() const;

private:
  NodeType* root;
  NodeType* fake_root;
  Compare comp;
  size_t size_;

  void setRoot(NodeType* root) noexcept;
  NodeType* getRoot() const noexcept;
  NodeType* copy_tree(NodeType* node, NodeType* parent);
  void clear(NodeType* NodeType);
  NodeType* find_NodeType(const Key& key) const;
  NodeType* find_min(NodeType* NodeType) const;
  NodeType* find_max(NodeType* NodeType) const;
  void transplant(NodeType* u, NodeType* v);

  void update_height(NodeType* NodeType);
  int get_balance_factor(NodeType* NodeType) const;
  int get_height(NodeType* node) const;
  NodeType* rotate_right(NodeType* y);
  NodeType* rotate_left(NodeType* x);
  NodeType* balance(NodeType* NodeType);
  NodeType* rebalance_path(std::stack< NodeType* >& path);
};

template< class Key, class Value >
using NodeType = Node< Key, Value >;

template < class Key, class Value, class Compare >
Tree< Key, Value, Compare >::Tree():
  fake_root(new NodeType(Key(), Value(), nullptr)),
  comp(Compare()),
  size_(0)
{
  fake_root->left = fake_root;
  fake_root->right = fake_root;
  setRoot(fake_root);
}

template < class Key, class Value, class Compare >
Tree< Key, Value, Compare >::Tree(const Compare& cmp):
  fake_root(new NodeType(Key(), Value(), nullptr)),
  comp(cmp),
  size_(0)
{
  fake_root->left = fake_root;
  fake_root->right = fake_root;
  setRoot(fake_root);
}

template < class Key, class Value, class Compare >
Tree< Key, Value, Compare >::Tree(const Tree& other):
  fake_root(new NodeType(Key(), Value(), nullptr)),
  comp(other.comp),
  size_(0)
{
  setRoot(copy_tree(other.getRoot(), fake_root));
  size_ = other.size_;
}

template < class Key, class Value, class Compare >
Tree< Key, Value, Compare >::Tree(Tree&& other) noexcept:
  root(other.root),
  fake_root(other.fake_root),
  comp(std::move(other.comp)),
  size_(other.size_)
{
  if (other.getRoot() == other.fake_root)
  {
    setRoot(fake_root);
  }
  other.fake_root = new NodeType(Key(), Value(), nullptr);
  other.size_ = 0;
}

template <class Key, class Value, class Compare >
Tree< Key, Value, Compare >::Tree(std::initializer_list< std::pair< const Key, Value > > init, const Compare& cmp):
  fake_root(new NodeType(Key(), Value(), nullptr)),
  comp(cmp),
  size_(0)
{
  setRoot(fake_root);
  for (const auto& item : init)
  {
    insert(item);
  }
}

template < class Key, class Value, class Compare >
template < class InputIt >
Tree< Key, Value, Compare >::Tree( InputIt first, InputIt last, const Compare& cmp):
  fake_root(new NodeType(Key(), Value(), nullptr)),
  comp(cmp),
  size_(0)
{
  setRoot(fake_root);
  for (; first != last; ++first)
  {
    insert(*first);
  }
}

template < class Key, class Value, class Compare >
Tree< Key, Value, Compare >::~Tree()
{
  clear();
  delete fake_root;
}

template < class Key, class Value, class Compare >
void Tree< Key, Value, Compare >::update_height(NodeType* NodeType)
{
  if (!NodeType || NodeType == fake_root)
  {
    return;
  }
  int left_height = NodeType->left ? NodeType->left->height : 0;
  int right_height = NodeType->right ? NodeType->right->height : 0;
  NodeType->height = 1 + std::max(left_height, right_height);
}


template <class Key, class Value, class Compare>
typename Tree<Key, Value, Compare>::NodeType*
Tree<Key, Value, Compare>::rotate_right(NodeType* y)
{
  if (y == fake_root || y->left == fake_root)
  {
    return y;
  }
  NodeType* x = y->left;
  NodeType* T2 = x->right;
  x->right = y;
  y->left = T2;
  x->parent = y->parent;
  y->parent = x;
  if (T2 != fake_root)
  {
    T2->parent = y;
  }

  return x;
}

template < class Key, class Value, class Compare >
class Tree< Key, Value, Compare >::NodeType*
Tree< Key, Value, Compare >::rotate_left(NodeType* x)
{
  if (!x || x == fake_root)
  {
    return x;
  }

  NodeType* y = x->right;
  x->right = y->left;

  if (y->left && y->left != fake_root)
  {
    y->left->parent = x;
  }

  y->left = x;
  y->parent = x->parent;
  x->parent = y;

  update_height(x);
  update_height(y);

  return y;
}
template <class Key, class Value, class Compare>
int Tree<Key, Value, Compare>::get_height(NodeType* node) const
{
  if (node == nullptr || node == fake_root)
  {
    return -1;
  }

  std::stack<NodeType*> nodes;
  std::unordered_map<NodeType*, int> heights;
  nodes.push(node);
  heights[nullptr] = -1;
  heights[fake_root] = -1;

  while (!nodes.empty())
  {
    NodeType* current = nodes.top();
    if (heights.find(current->left) != heights.end() && heights.find(current->right) != heights.end())
    {
      heights[current] = 1 + std::max(heights[current->left], heights[current->right]);
      nodes.pop();
    }
    else
    {
      if (heights.find(current->right) == heights.end())
      {
        nodes.push(current->right);
      }
      if (heights.find(current->left) == heights.end())
      {
        nodes.push(current->left);
      }
    }
  }

  return heights[node];
}

template <class Key, class Value, class Compare>
int Tree<Key, Value, Compare>::get_balance_factor(NodeType* node) const
{
  if (node == nullptr || node == fake_root)
  {
    return 0;
  }

  int left_height = (node->left && node->left != fake_root) ? get_height(node->left) : -1;
  int right_height = (node->right && node->right != fake_root) ? get_height(node->right) : -1;

  return left_height - right_height;
}

template <class Key, class Value, class Compare>
typename Tree<Key, Value, Compare>::NodeType*
Tree<Key, Value, Compare>::balance(NodeType* node)
{
  if (node == nullptr || node == fake_root)
  {
    return node;
  }

  int balance_factor = get_balance_factor(node);

  if (balance_factor > 1)
  {
    if (get_balance_factor(node->left) >= 0)
    {
      return rotate_right(node);
    }
    else
    {
      node->left = rotate_left(node->left);
      return rotate_right(node);
    }
  }
  else if (balance_factor < -1)
  {
    if (get_balance_factor(node->right) <= 0)
    {
      return rotate_left(node);
    }
    else
    {
      node->right = rotate_right(node->right);
      return rotate_left(node);
    }
  }

  return node;
}

template <class Key, class Value, class Compare>
void Tree<Key, Value, Compare>::push(const Key& key, const Value& value)
{
  if (fake_root->left == fake_root)
  {
    NodeType* new_node = new NodeType(key, value, fake_root);
    fake_root->left = new_node;
    fake_root->right = new_node;
    size_++;
    return;
  }

  std::stack<NodeType*> path;
  NodeType* current = fake_root->left;
  NodeType* parent = fake_root;

  while (current != fake_root)
  {
    parent = current;
    path.push(current);

    if (comp(key, current->data.first))
    {
      current = current->left;
    }
    else if (comp(current->data.first, key))
    {
      current = current->right;
    }
    else
    {
      return;
    }
  }

  NodeType* new_node = new NodeType(key, value, parent);
  if (comp(key, parent->data.first))
  {
    parent->left = new_node;
  }
  else
  {
    parent->right = new_node;
  }

  path.push(new_node);
  rebalance_path(path);
  size_++;
}

template <class Key, class Value, class Compare>
typename Tree<Key, Value, Compare>::NodeType*
Tree<Key, Value, Compare>::rebalance_path(std::stack<NodeType*>& path)
{
  NodeType* new_root = nullptr;
  while (!path.empty())
  {
    NodeType* node = path.top();
    path.pop();
    NodeType* balanced_node = balance(node);
    if (!path.empty())
    {
      NodeType* parent_node = path.top();
      if (parent_node->left == node)
      {
        parent_node->left = balanced_node;
      }
      else
      {
        parent_node->right = balanced_node;
      }
    }
    else
    {
      new_root = balanced_node;
    }
  }
  if (new_root)
  {
    fake_root->left = new_root;
    new_root->parent = fake_root;
  }
  return new_root;
}


template < class Key, class Value, class Compare >
Tree< Key, Value, Compare >& Tree< Key, Value, Compare >::operator=(const Tree& other)
{
  if (this != &other)
  {
    clear();
    setRoot(copy_tree(other.getRoot(), fake_root));
    size_ = other.size_;
  }
  return *this;
}

template < class Key, class Value, class Compare >
Tree< Key, Value, Compare >& Tree< Key, Value, Compare >::operator=(Tree&& other) noexcept
{
  if (this != &other)
  {
    clear();
    delete fake_root;
    fake_root = other.fake_root;
    size_ = other.size_;
    other.fake_root = new NodeType(Key(), Value(), nullptr);
    other.size_ = 0;
  }
  return *this;
}

template < class Key, class Value, class Compare >
bool Tree< Key, Value, Compare >::empty() const
{
  return size_ == 0;
}

template < class Key, class Value, class Compare >
size_t Tree< Key, Value, Compare >::size() const
{
  return size_;
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::iterator Tree< Key, Value, Compare >::begin()
{
  if (!getRoot() || getRoot() == fake_root)
  {
    return end();
  }
  return iterator(find_min(getRoot()));
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::iterator Tree< Key, Value, Compare >::end()
{
  return iterator(fake_root);
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::const_iterator Tree< Key, Value, Compare >::begin() const
{
  if (!getRoot() || fake_root == getRoot())
  {
    return end();
  }
  return const_iterator(find_min(getRoot()));
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::const_iterator Tree< Key, Value, Compare >::end() const
{
  return const_iterator(fake_root);
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::const_iterator Tree< Key, Value, Compare >::cbegin() const
{
  return begin();
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::const_iterator Tree< Key, Value, Compare >::cend() const
{
  return end();
}

template < class Key, class Value, class Compare >
void Tree< Key, Value, Compare >::clear()
{
  if (getRoot() && getRoot() != fake_root)
  {
    clear(getRoot());
    setRoot(fake_root);
  }
  size_ = 0;
}

template < class Key, class Value, class Compare >
void Tree< Key, Value, Compare >::clear(NodeType* node)
{
  if (!node || node == fake_root)
  {
    return;
  }
  clear(node->left);
  clear(node->right);
  delete node;
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::NodeType*
Tree< Key, Value, Compare >::find_NodeType(const Key& key) const
{
  NodeType* current = getRoot();
  while (current && current != fake_root)
  {
    if (comp(key, current->data.first))
    {
      current = current->left;
    }
    else if (comp(current->data.first, key))
    {
      current = current->right;
    }
    else
    {
      return current;
    }
  }
  return nullptr;
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::NodeType*
Tree< Key, Value, Compare >::find_min(NodeType* NodeType) const
{
  while (NodeType->left && NodeType->left != fake_root)
  {
    NodeType = NodeType->left;
  }
  return NodeType;
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::NodeType*
Tree< Key, Value, Compare >::find_max(NodeType* NodeType) const
{
  while (NodeType->right && NodeType->right != fake_root)
  {
    NodeType = NodeType->right;
  }
  return NodeType;
}

template < class Key, class Value, class Compare >
void Tree< Key, Value, Compare >::transplant(NodeType* u, NodeType* v)
{
  if (u->parent == fake_root)
  {
    root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }

  if (v && v != fake_root)
  {
    v->parent = u->parent;
  }
}


template < class Key, class Value, class Compare >
void Tree< Key, Value, Compare >::swap(Tree< Key, Value, Compare >& other) noexcept
{
  if (this != &other)
  {
    std::swap(fake_root, other.fake_root);
    std::swap(comp, other.comp);
    std::swap(size_, other.size_);
  }
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::iterator
Tree< Key, Value, Compare >::find(const Key& key)
{
  NodeType* NodeType = find_NodeType(key);
  if (NodeType && NodeType != fake_root)
  {
    return iterator(NodeType);
  }
  return end();
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::const_iterator
Tree< Key, Value, Compare >::find(const Key& key) const
{
  const NodeType* NodeType = find_NodeType(key);
  if (NodeType && NodeType != fake_root)
  {
    return const_iterator(NodeType);
  }
  return end();
}

template < class Key, class Value, class Compare >
size_t Tree< Key, Value, Compare >::count(const Key& key) const
{
  return find(key) != end() ? 1 : 0;
}

template < class Key, class Value, class Compare >
std::pair< typename Tree< Key, Value, Compare >::iterator, typename Tree< Key, Value, Compare >::iterator >
Tree< Key, Value, Compare >::equal_range(const Key& key)
{
  return { lower_bound(key), upper_bound(key) };
}

template < class Key, class Value, class Compare >
std::pair< typename Tree< Key, Value, Compare >::const_iterator, typename Tree< Key, Value, Compare >::const_iterator >
Tree< Key, Value, Compare >::equal_range(const Key& key) const
{
  return { lower_bound(key), upper_bound(key) };
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::iterator
Tree< Key, Value, Compare >::lower_bound(const Key& key)
{
  NodeType* current = getRoot();
  NodeType* result = fake_root;

  while (current && current != fake_root)
  {
    if (!comp(current->data.first, key))
    {
      result = current;
      current = current->left;
    }
    else
    {
      current = current->right;
    }
  }
  return iterator(result);
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::const_iterator
Tree< Key, Value, Compare >::lower_bound(const Key& key) const
{
  const NodeType* current = getRoot();
  const NodeType* result = fake_root;

  while (current && current != fake_root)
  {
    if (!comp(current->data.first, key))
    {
      result = current;
      current = current->left;
    }
    else
    {
      current = current->right;
    }
  }
  return const_iterator(result);
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::iterator
Tree< Key, Value, Compare >::upper_bound(const Key& key)
{
  NodeType* current = getRoot();
  NodeType* result = fake_root;

  while (current && current != fake_root)
  {
    if (comp(key, current->data.first))
    {
      result = current;
      current = current->left;
    }
    else
    {
      current = current->right;
    }
  }
  return iterator(result);
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::const_iterator
Tree< Key, Value, Compare >::upper_bound(const Key& key) const
{
  const NodeType* current = getRoot();
  const NodeType* result = fake_root;

  while (current && current != fake_root)
  {
    if (comp(key, current->data.first))
    {
      result = current;
      current = current->left;
    }
    else
    {
      current = current->right;
    }
  }
  return const_iterator(result);
}


template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::iterator
Tree< Key, Value, Compare >::insert(const std::pair< const Key, Value >& value)
{
  push(value.first, value.second);
  return iterator(find(value.first));
}

template < class Key, class Value, class Compare >
template <typename InputIt>
void Tree< Key, Value, Compare >::insert(InputIt first, InputIt last)
{
  for (; first != last; ++first)
  {
    insert(*first);
  }
}

template < class Key, class Value, class Compare >
void Tree< Key, Value, Compare >::insert(std::initializer_list< std::pair< const Key, Value > > ilist)
{
  for (const auto& item : ilist)
  {
    insert(item);
  }
}

template <class Key, class Value, class Compare>
typename Tree<Key, Value, Compare>::iterator
Tree<Key, Value, Compare>::erase(iterator pos)
{
  if (pos == end() || pos.current == fake_root)
  {
    return end();
  }
  NodeType* NodeTypeo_erase = pos.node;
  NodeType* parent = NodeTypeo_erase->parent;
  std::stack<NodeType*> path;
  if (NodeTypeo_erase->left == fake_root && NodeTypeo_erase->right == fake_root)
  {
    if (parent->left == NodeTypeo_erase)
    {
      parent->left = fake_root;
    }
    else
    {
      parent->right = fake_root;
    }
    path.push(parent);
  }
  else if (NodeTypeo_erase->left == fake_root || NodeTypeo_erase->right == fake_root)
  {
    NodeType* child = nullptr;
    if (NodeTypeo_erase->left == fake_root)
    {
      child = NodeTypeo_erase->right;
    }
    else
    {
      child = NodeTypeo_erase->left;
    }
    if (parent->left == NodeTypeo_erase)
    {
      parent->left = child;
    }
    else
    {
      parent->right = child;
    }
    child->parent = parent;
    path.push(parent);
  }
  else
  {
    NodeType* min_right = NodeTypeo_erase->right;
    while (min_right->left != fake_root)
    {
      min_right = min_right->left;
    }
    NodeTypeo_erase->data = min_right->data;
    parent = min_right->parent;
    if (parent->left == min_right)
    {
      parent->left = (min_right->right != fake_root) ? min_right->right : fake_root;
    }
    else
    {
      parent->right = (min_right->right != fake_root) ? min_right->right : fake_root;
    }
    if (min_right->right != fake_root)
    {
      min_right->right->parent = parent;
    }
    path.push(parent);
    NodeTypeo_erase = min_right;
  }
  NodeType* new_root = rebalance_path(path);
  if (new_root != nullptr)
  {
    fake_root->left = new_root;
    new_root->parent = fake_root;
  }
  delete NodeTypeo_erase;
  size_--;
  return iterator(parent);
}
template < class Key, class Value, class Compare >
size_t Tree< Key, Value, Compare >::erase(const Key& key)
{
  auto it = find(key);
  if (it != end())
  {
    erase(it);
    return 1;
  }
  return 0;
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::iterator
Tree< Key, Value, Compare >::erase(iterator first, iterator last)
{
  while (first != last)
  {
    first = erase(first);
  }
  return last;
}

template < class Key, class Value, class Compare >
Value& Tree< Key, Value, Compare >::at(const Key& key)
{
  NodeType* NodeType = find_NodeType(key);
  if (!NodeType || NodeType == fake_root)
  {
    throw std::out_of_range("not found");
  }
  return NodeType->data.second;
}

template < class Key, class Value, class Compare >
const Value& Tree< Key, Value, Compare >::at(const Key& key) const
{
  const NodeType* NodeType = find_NodeType(key);
  if (!NodeType || NodeType == fake_root)
  {
    throw std::out_of_range("not found");
  }
  return NodeType->data.second;
}

template < class Key, class Value, class Compare >
Value& Tree< Key, Value, Compare >::operator[](const Key& key)
{
  auto result = insert(std::make_pair(key, Value()));
  return result.first->second;
}

template < class Key, class Value, class Compare >
const Value& Tree< Key, Value, Compare >::operator[](const Key& key) const
{
  return at(key);
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::NodeType*
Tree< Key, Value, Compare >::copy_tree(NodeType* node, NodeType* parent)
{
  if (!node || node == fake_root)
  {
    return nullptr;
  }

  NodeType* new_node = new NodeType(node->data.first, node->data.second, parent);
  new_node->left = copy_tree(node->left, new_node);
  new_node->right = copy_tree(node->right, new_node);
  new_node->height = node->height;
  return balance(new_node);
}

template < class Key, class Value, class Compare >
void Tree< Key, Value, Compare >::setRoot(NodeType* root) noexcept
{
  fake_root->left = root;
  if (root)
  {
    root->parent = fake_root;
  }
}

template < class Key, class Value, class Compare >
typename Tree< Key, Value, Compare >::NodeType*
Tree< Key, Value, Compare >::getRoot() const noexcept
{
  return fake_root->left;
}

#endif
