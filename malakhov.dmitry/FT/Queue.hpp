#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <algorithm>
#include <stdexcept>

#include "Deque.hpp"

namespace malakhov
{
  using ValueT = long long int;

  template < typename ContainmentT >
  class Queue
  {
  public:
    Queue():
      storage_()
    {}

    Queue(const Queue< ContainmentT >& instance) = default;

    ~Queue() = default;

    void swap(Queue& instance) noexcept
    {
      storage_.swap(instance.storage);
      std::swap(instance.freeFrontSpace_);
    }

    void resize(const size_t newSize)
    {
      if (newSize < getUsedSpace())
      {
        throw std::logic_error("Queue: New size cannot be lesser than old one");
      }
      storage_.resize(newSize);
    }

    void reserve(const size_t newSize)
    {
      storage_.reserveRight(newSize);
    }

    void shrinkToFit()
    {
      storage_.shrinkToFit();
    }

    void operator=(const Queue< ContainmentT >& instance)
    {
      Queue< ContainmentT > result;
      swap(result);
    }

    bool isEmpty() const
    {
      return !getUsedSpace();
    }

    void putIn(const ContainmentT& instance)
    {
      storage_.pushBack(instance);
    }

    void popOut()
    {
      storage_.popFront();
    }

    const ContainmentT& getFirst() const
    {
      if (!getUsedSpace())
      {
        throw std::logic_error("Queue: nothing to get");
      }
      return storage_[0];
    }

    size_t getSize() const
    {
      return storage_.getSize();
    }

    size_t getUsedSpace() const
    {
      return storage_.getUsedSpace();
    }

  private:
    Deque< ContainmentT > storage_;
  };
}

#endif
