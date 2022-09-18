#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <algorithm>
#include <stdexcept>

#include "Array.hpp"

namespace malakhov
{
  template < typename ContainmentT >
  class Stack
  {
  public:
    Stack():
      storage_()
    {}

    Stack(const Stack< ContainmentT >& instance) = default;

    ~Stack() = default;

    void swap(Stack& instance) noexcept
    {
      storage_.swap(instance.storage_);
    }

    void resize(const size_t newSize)
    {
      if (newSize < storage_.getUsedSpace())
      {
        throw std::logic_error("Stack: New size cannot be lesser than the old one");
      }
      storage_.resize(newSize);
    }

    void reserve(const size_t newSize)
    {
      storage_.reserve(newSize);
    }

    void shrinkToFit()
    {
      storage_.shrinkToFit();
    }

    void operator=(const Stack< ContainmentT >& instance)
    {
      Stack< ContainmentT > result{instance};
      swap(result);
    }

    bool isEmpty() const
    {
      return !storage_.getUsedSpace();
    }

    void putIn(const ContainmentT& instance)
    {
      storage_.pushBack(instance);
    }

    void popOut()
    {
      storage_.popBack();
    }

    const ContainmentT& getLast() const
    {
      if (!getUsedSpace())
      {
        throw std::logic_error("Stack: No items to get");
      }
      return storage_[getUsedSpace() - 1];
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
    detail::Array< ContainmentT > storage_;
  };
}

#endif
