#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP
#include <cstddef>
#include <stdexcept>
#include "node.hpp"
#include "constIterator.hpp"

namespace smirnov
{
  template < typename Key, typename Value, typename Compare = std::less< Key > >
  class AvlTree
  {
  public:
    using const_iterator = ConstIterator< Key, Value, Compare >;
    AvlTree();
    AvlTree(const AvlTree & other);
    AvlTree(AvlTree && other) noexcept;
    ~AvlTree();
    AvlTree & operator=(const AvlTree & other);
    AvlTree & operator=(AvlTree && other) noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear() noexcept;
    void swap(AvlTree & other) noexcept;
    Value & operator[](const Key & key);
    const Value & operator[](const Key & key) const;
    Value & at(const Key & key);
    const Value & at(const Key & key) const;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_iterator find(const Key & key) const noexcept;
  private:
    Node< Key, Value > * root_;
    size_t size_;
    Compare comp_;
    void clearNode(Node< Key, Value > * node);
    int height(Node< Key, Value > * node) const;
    void updateHeight(Node< Key, Value > * node);
    int getBalanceFactor(Node< Key, Value > * node) const;
    Node< Key, Value > * rotateLeft(Node< Key, Value > * node);
    Node< Key, Value > * rotateRight(Node< Key, Value > * node);
    Node< Key, Value > * balance(Node< Key, Value > * node);
    Node< Key, Value > * addOrGet(Node< Key, Value > * node, const Key & key, const Value & value, size_t & size);
    Node< Key, Value > * findNode(Node< Key, Value > * node, const Key & key) const;
    Node< Key, Value > * minNode(Node< Key, Value > * node) const;
  };

  template < typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare >::AvlTree():
    root_(nullptr),
    size_(0),
    comp_(Compare())
  {}

  template < typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare >::AvlTree(const AvlTree & other):
    root_(nullptr),
    size_(0),
    comp_(other.comp_)
  {
    for (auto it = other.cbegin(); it != other.cend(); ++it)
    {
      operator[](it->first) = it->second;
    }
  }

  template < typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare >::AvlTree(AvlTree && other) noexcept:
    root_(other.root_),
    size_(other.size_),
    comp_(std::move(other.comp_))
  {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  template < typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare >::~AvlTree()
  {
    clearNode(root_);
    root_ = nullptr;
    size_ = 0;
  }

  template < typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare > & AvlTree< Key, Value, Compare >::operator=(const AvlTree & other)
  {
    if (this != std::addressof(other))
    {
      clear();
      comp_ = other.comp_;
      for (auto it = other.cbegin(); it != other.cend(); ++it)
      {
        operator[](it->first) = it->second;
      }
    }
    return *this;
  }

  template < typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare > & AvlTree< Key, Value, Compare >::operator=(AvlTree && other) noexcept
  {
    if (this != std::addressof(other))
    {
      clearNode(root_);
      root_ = other.root_;
      size_ = other.size_;
      comp_ = std::move(other.comp_);
      other.root_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  template < typename Key, typename Value, typename Compare >
  bool AvlTree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < typename Key, typename Value, typename Compare >
  size_t AvlTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template < typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::clear() noexcept
  {
    clearNode(root_);
    root_ = nullptr;
    size_ = 0;
  }

  template < typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::swap(AvlTree & other) noexcept
  {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
    std::swap(comp_, other.comp_);
  }

  template < typename Key, typename Value, typename Compare >
  Value & AvlTree< Key, Value, Compare >::operator[](const Key & key)
  {
    root_ = addOrGet(root_, key, Value(), size_);
    Node< Key, Value > * node = findNode(root_, key);
    return std::addressof(node->data)->second;
  }

  template < typename Key, typename Value, typename Compare >
  const Value & AvlTree< Key, Value, Compare >::operator[](const Key & key) const
  {
    Node< Key, Value > * node = findNode(root_, key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return std::addressof(node->data)->second;
  }

  template < typename Key, typename Value, typename Compare >
  Value & AvlTree< Key, Value, Compare >::at(const Key & key)
  {
    Node< Key, Value > * node = findNode(root_, key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return std::addressof(node->data)->second;
  }

  template < typename Key, typename Value, typename Compare >
  const Value & AvlTree< Key, Value, Compare >::at(const Key & key) const
  {
    Node< Key, Value > * node = findNode(root_, key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return std::addressof(node->data)->second;
  }

  template < typename Key, typename Value, typename Compare >
  typename AvlTree < Key, Value, Compare >::const_iterator AvlTree< Key, Value, Compare >::cbegin() const noexcept
  {
    return const_iterator(minNode(root_));
  }

  template < typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::const_iterator AvlTree< Key, Value, Compare >::cend() const noexcept
  {
    return const_iterator(nullptr);
  }

  template < typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::const_iterator AvlTree< Key, Value, Compare >::find(const Key & key) const noexcept
  {
    Node< Key, Value > * node = findNode(root_, key);
    if (node)
    {
      return const_iterator(node);
    }
    return cend();
  }

  template < typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::clearNode(Node< Key, Value > * node)
  {
    if (!node)
    {
      return;
    }
    clearNode(node->left);
    clearNode(node->right);
    delete node;
  }

  template < typename Key, typename Value, typename Compare >
  int AvlTree< Key, Value, Compare >::height(Node< Key, Value > * node) const
  {
    if (node == nullptr)
    {
      return 0;
    }
    else
    {
      return node->height;
    }
  }

  template < typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::updateHeight(Node< Key, Value > * node)
  {
    int leftH = height(node->left);
    int rightH = height(node->right);
    if (leftH > rightH)
    {
      node->height = leftH + 1;
    }
    else
    {
      node->height = rightH + 1;
    }
  }

  template < typename Key, typename Value, typename Compare >
  int AvlTree< Key, Value, Compare >::getBalanceFactor(Node< Key, Value > * node) const
  {
    return height(node->left) - height(node->right);
  }

  template < typename Key, typename Value, typename Compare >
  Node< Key, Value > * AvlTree< Key, Value, Compare >::rotateLeft(Node< Key, Value > * node)
  {
    Node< Key, Value > * newRoot = node->right;
    node->right = newRoot->left;
    if (newRoot->left != nullptr)
    {
      newRoot->left->parent = node;
    }
    newRoot->left = node;
    newRoot->parent = node->parent;
    node->parent = newRoot;
    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
  }

  template < typename Key, typename Value, typename Compare >
  Node< Key, Value > * AvlTree< Key, Value, Compare >::rotateRight(Node< Key, Value > * node)
  {
    Node< Key, Value > * newRoot = node->left;
    node->left = newRoot->right;
    if (newRoot->right != nullptr)
    {
      newRoot->right->parent = node;
    }
    newRoot->right = node;
    newRoot->parent = node->parent;
    node->parent = newRoot;
    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
  }

  template < typename Key, typename Value, typename Compare >
  Node< Key, Value > * AvlTree< Key, Value, Compare >::balance(Node< Key, Value > * node)
  {
    updateHeight(node);
    int balanceFactor = getBalanceFactor(node);
    if (balanceFactor > 1)
    {
      if (getBalanceFactor(node->left) < 0)
      {
        node->left = rotateLeft(node->left);
      }
      return rotateRight(node);
    }
    if (balanceFactor < -1)
    {
      if (getBalanceFactor(node->right) > 0)
      {
        node->right = rotateRight(node->right);
      }
      return rotateLeft(node);
    }
    return node;
  }

  template < typename Key, typename Value, typename Compare >
  Node< Key, Value > * AvlTree< Key, Value, Compare >::addOrGet(
      Node< Key, Value > * node, const Key & key, const Value & value, size_t & size)
  {
    if (!node)
    {
      ++size;
      return new NodeType(key, value);
    }
    if (comp_(key, node->data.first))
    {
      Node< Key, Value > * leftChild = addOrGet(node->left, key, value, size);
      node->left = leftChild;
      leftChild->parent = node;
    }
    else if (comp_(node->data.first, key))
    {
      Node< Key, Value > * rightChild = addOrGet(node->right, key, value, size);
      node->right = rightChild;
      rightChild->parent = node;
    }
    else
    {
      node->data.second = value;
    }
    return balance(node);
  }

  template < typename Key, typename Value, typename Compare >
  Node< Key, Value > * AvlTree< Key, Value, Compare >::findNode(Node< Key, Value > * node, const Key & key) const
  {
    if (!node)
    {
      return nullptr;
    }
    if (comp_(key, node->data.first))
    {
      return findNode(node->left, key);
    }
    else if (comp_(node->data.first, key))
    {
      return findNode(node->right, key);
    }
    return node;
  }

  template < typename Key, typename Value, typename Compare >
  Node< Key, Value > * AvlTree< Key, Value, Compare >::minNode(Node< Key, Value > * node) const
  {
    if (!node)
    {
      return nullptr;
    }
    while (node->left)
    {
      node = node->left;
    }
    return node;
  }
}
#endif
