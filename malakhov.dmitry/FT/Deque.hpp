#ifndef DEQUE_HPP
#define DEQUE_HPP

#include "Array.hpp"

namespace malakhov
{
  using ValueT = long long int;

  template < typename ContainmentT >
  class Deque
  {
  public:
    Deque():
      head_(),
      tail_()
    {}

    Deque(const Deque& Deque) = default;

    ~Deque() = default;

    void pushBack(const ContainmentT& obj)
    {
      tail_.pushBack(obj);
    }

    void popBack()
    {
      if (!tail_.getUsedSpace())
      {
        shiftAllToRight();
      }
      tail_.popBack();
    }

    void pushFront(const ContainmentT& obj)
    {
      head_.pushBack(obj);
    }

    void popFront()
    {
      if (!head_.getUsedSpace())
      {
        shiftAllToLeft();
      }
      head_.popBack();
    }

    void swap(Deque& instance) noexcept
    {
      head_.swap(instance.head_);
      tail_.swap(instance.tail_);
    }

    void shrinkToFit()
    {
      tail_.shrinkToFit();
      head_.shrinkToFit();
    }

    size_t getSize() const
    {
      return tail_.getSize() + head_.getSize();
    }

    size_t getUsedSpace() const
    {
      return tail_.getUsedSpace() + head_.getUsedSpace();
    }

    ContainmentT& operator[](const size_t index)
    {
      if (index >= head_.getUsedSpace())
      {
        return tail_[index - head_.getUsedSpace()];
      }
      else
      {
        return head_[head_.getUsedSpace() - 1 - index];
      }
    }

    const ContainmentT& operator[](const size_t index) const
    {
      if (index >= head_.getUsedSpace())
      {
        return tail_[index - head_.getUsedSpace()];
      }
      else
      {
        return head_[head_.getUsedSpace() - 1 - index];
      }
    }

    void operator=(const Deque& instance)
    {
      Deque result{instance};
      swap(*this, result);
    }

    void shiftAllToLeft()
    {
      Deque< ContainmentT > result;
      result.reserveLeft(getUsedSpace());
      result.reserveRight(tail_.getSize());
      for (size_t i = getUsedSpace() - 1; i > 0; --i)
      {
        result.pushFront((*this)[i]);
      }
      result.pushFront((*this)[0]);
      swap(result);
    }

    void shiftAllToRight()
    {
      Deque< ContainmentT > result;
      result.reserveRight(getUsedSpace());
      result.reserveLeft(head_.getSize());
      for (size_t i = 0; i < getUsedSpace(); ++i)
      {
        result.pushBack((*this)[i]);
      }
      swap(result);
    }

    void reserveLeft(size_t newSize)
    {
      head_.reserve(newSize);
    }

    void reserveRight(size_t newSize)
    {
      tail_.reserve(newSize);
    }

  private:
    detail::Array< ContainmentT > head_;
    detail::Array< ContainmentT > tail_;
  };
}

#endif
