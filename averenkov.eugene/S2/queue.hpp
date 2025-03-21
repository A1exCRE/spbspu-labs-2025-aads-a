#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "array.hpp"

namespace averenkov
{
  template< class T >
  class Queue
  {
  public:
    Queue() = default;
    ~Queue() = default;
    Queue(const Queue &rhs);
    Queue(Queue &&rhs) noexcept;
    Queue &operator=(const Queue &rhs);
    bool empty() const noexcept;
    size_t size() const noexcept;

    const T& front() const noexcept;
    T& front() noexcept;
    const T& back() const noexcept;
    T& back() noexcept;

    void push(T rhs);
    T drop();

  private:
    Array< T > arr_;
  };

  template< class T >
  Queue< T >::Queue(const Queue &rhs):
    arr_(rhs.arr_)
  {}

  template< class T >
  Queue< T >::Queue(Queue &&rhs) noexcept:
    arr_(std::move(rhs.arr_))
  {}

  template< class T >
  Queue< T > &Queue< T >::operator=(const Queue &rhs)
  {
    if (this != &rhs)
    {
      arr_ = rhs.arr_;
    }
    return *this;
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return arr_.empty();
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return arr_.size();
  }

  template< class T >
  const T& Queue< T >::front() const noexcept
  {
    return arr_.front();
  }

  template< class T >
  T& Queue< T >::front() noexcept
  {
    return arr_.front();
  }

  template< class T >
  const T& Queue< T >::back() const noexcept
  {
    return arr_.back();
  }

  template< class T >
  T& Queue< T >::back() noexcept
  {
    return arr_.back();
  }

  template< class T >
  void Queue< T >::push(T rhs)
  {
    arr_.push_back(rhs);
  }

  template< class T >
  T Queue< T >::drop()
  {
    T temp = front();
    arr_.pop_front();
    return temp;
  }

}

#endif
