#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <cstddef>
#include <stdexcept>

namespace malakhov
{
  namespace detail
  {
    const char* const ARR_MEMORY_ACCESS_RESTRICTED = "Array: memory access restricted";

    template < typename ContainmentT >
    class Array
    {
    public:
      Array():
        storage_(nullptr),
        capacity_(0),
        usedSpace_(0)
      {}

      Array(const Array& arr):
        storage_(arr.usedSpace_ ? allocateStorage(usedSpace_) : nullptr),
        capacity_(arr.usedSpace_),
        usedSpace_(arr.usedSpace_)
      {
        for (size_t i = 0; i < usedSpace_; ++i)
        {
          new (storage_ + i) ContainmentT(arr[i]);
        }
      }

      ~Array()
      {
        deallocateStorage();
      }

      void pushBack(const ContainmentT& obj)
      {
        reserve(usedSpace_ + 1);
        new (storage_ + usedSpace_) ContainmentT(obj);
        ++usedSpace_;
      }

      void popBack()
      {
        if (!usedSpace_)
        {
          throw std::logic_error("No items inside");
        }
        storage_->~ContainmentT();
        --usedSpace_;
      }

      void swap(Array& arr) noexcept
      {
        std::swap(storage_, arr.storage_);
        std::swap(capacity_, arr.capacity_);
        std::swap(usedSpace_, arr.usedSpace_);
      }

      void reserve(const size_t newSize)
      {
        if (newSize <= capacity_)
        {
          return;
        }
        modifyStorageSize(newSize);
      }

      void shrinkToFit()
      {
        modifyStorageSize(usedSpace_);
      }

      ContainmentT* getcBcegin() const
      {
        return storage_;
      }

      ContainmentT* getcEnd() const
      {
        return storage_ + usedSpace_;
      }

      size_t getSize() const
      {
        return capacity_;
      }

      size_t getUsedSpace() const
      {
        return usedSpace_;
      }

      ContainmentT& operator[](const size_t index)
      {
        if (index >= usedSpace_)
        {
          throw std::out_of_range(detail::ARR_MEMORY_ACCESS_RESTRICTED);
        }
        return storage_[index];
      }

      const ContainmentT& operator[](const size_t index) const
      {
        if (index >= usedSpace_)
        {
          throw std::out_of_range(detail::ARR_MEMORY_ACCESS_RESTRICTED);
        }
        return storage_[index];
      }

      void operator=(const Array< ContainmentT >& arr)
      {
        Array result{arr};
        swap(result);
      }

    private:
      ContainmentT* allocateStorage(size_t count)
      {
        return static_cast< ContainmentT* >(operator new(count * sizeof(ContainmentT)));
      }

      void deallocateStorage()
      {
        if (storage_)
        {
          for (size_t i = 0; i < usedSpace_; ++i)
          {
            storage_[i].~ContainmentT();
          }
          delete storage_;
        }
      }

      void modifyStorageSize(size_t newSize)
      {
        if (!newSize)
        {
          deallocateStorage();
          return;
        }
        ContainmentT* newStorage = allocateStorage(newSize);
        try
        {
          for (size_t i = 0; i < usedSpace_; ++i)
          {
            new (newStorage + i) ContainmentT(storage_[i]);
          }
        }
        catch (...)
        {
          for (size_t i = 0; i < usedSpace_; ++i)
          {
            newStorage[i].~ContainmentT();
          }
          delete newStorage;
          throw;
        }
        deallocateStorage();
        storage_ = newStorage;
        capacity_ = newSize;
      }

      ContainmentT* storage_;
      size_t capacity_;
      size_t usedSpace_;
    };
  }
}

#endif
