#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP

#include <functional>
#include <memory>
#include <stdexcept>

namespace malakhov
{
  namespace detail
  {
    const char* const NOT_FOUND_EXCEPTION = "BST::operator[] : item was not found";

    template < typename KeyT, typename ValueT, typename ComparatorT = ::std::less< KeyT > >
    class BinarySearchTree
    {
    public:
      using PairT = std::pair< KeyT, ValueT >;

      struct Iterator;
      struct ConstIterator;

      struct Node
      {
      public:
        Node(const Node&) = delete;

        Node(const Node& instance, Node* up):
          stored(instance.stored),
          up(up),
          right(instance.right ? new Node{*instance.right, this} : nullptr),
          left(instance.left ? new Node{*instance.left, this} : nullptr)
        {}

        Node(const PairT& pair, Node* up, Node* right, Node* left):
          stored(pair),
          up(up),
          right(right),
          left(left)
        {}

        ~Node()
        {
          if (right)
          {
            delete right;
            right = nullptr;
          }
          if (left)
          {
            delete left;
            left = nullptr;
          }
        }

        Node* getMin() noexcept
        {
          Node* result = this;
          if (result)
          {
            while (result->left)
            {
              result = result->left;
            }
          }
          return result;
        }

        const Node* getMin() const noexcept
        {
          const Node* result = this;
          while (result->left)
          {
            result = result->left;
          }
          return result;
        }

        Node* getMax() noexcept
        {
          Node* result = this;
          while (result->right)
          {
            result = result->right;
          }
          return result;
        }

        const Node* getMax() const noexcept
        {
          const Node* result = this;
          while (result->right)
          {
            result = result->right;
          }
          return result;
        }

        Node* lTurn() noexcept
        {
          Node* node = this->right;
          this->right = node->left;
          node->left = this;
          node->up = this->up;
          this->up = node;
          if (this->right)
          {
            this->right->up = this;
          }
          return node;
        }

        Node* rTurn() noexcept
        {
          Node* node = this->left;
          this->left = node->right;
          node->right = this;
          node->up = this->up;
          this->up = node;
          if (this->left)
          {
            this->left->up = this;
          }
          return node;
        }

        size_t getHeight() const noexcept
        {
          const char rightH = right ? right->getHeight() : 0;
          const char leftH = left ? left->getHeight() : 0;
          return std::max(rightH, leftH) + 1;
        }

        size_t getSize() const noexcept
        {
          return 1 + (left ? left->getSize() : 0) + (right ? right->getSize() : 0);
        }

        char getBalanceFactor() const noexcept
        {
          const char rightH = right ? right->getHeight() : 0;
          const char leftH = left ? left->getHeight() : 0;
          return rightH - leftH;
        }

        PairT stored;
        Node* up;
        Node* right;
        Node* left;
      };

      struct Iterator: public std::iterator< std::bidirectional_iterator_tag, PairT >
      {
        friend ConstIterator;

      public:
        Iterator& operator++()
        {
          if (!ptr_)
          {
            return *this;
          }

          if (ptr_->right)
          {
            ptr_ = ptr_->right->getMin();
            return *this;
          }

          ComparatorT comparator;
          if (ptr_->up && comparator(ptr_->stored.first, ptr_->up->stored.first))
          {
            ptr_ = ptr_->up;
            return *this;
          }

          while (ptr_->up && comparator(ptr_->up->stored.first, ptr_->stored.first))
          {
            ptr_ = ptr_->up;
          }
          ptr_ = ptr_->up;
          return *this;
        }

        Iterator& operator--()
        {
          if (!ptr_)
          {
            return nullptr;
          }

          if (ptr_->left_)
          {
            ptr_ = ptr_->left_->getMax();
            return this;
          }

          ComparatorT comparator;
          if (ptr_->up && comparator(ptr_->up->stored.first, ptr_->stored.first))
          {
            ptr_ = ptr_->up;
            return this;
          }

          while (ptr_->up && comparator(ptr_->stored.first, ptr_->up->stored.first))
          {
            ptr_ = ptr_->up;
          }
          ptr_ = ptr_->up;
          return this;
        }

        PairT* operator->() const noexcept
        {
          return std::addressof(ptr_->stored);
        }

        PairT& operator*() const noexcept
        {
          return ptr_->stored;
        }

        bool operator==(const Iterator& it) const noexcept
        {
          return it.ptr_ == this->ptr_;
        }

        bool operator==(const ConstIterator& it) const noexcept
        {
          return it.ptr_ == this->ptr_;
        }

        bool operator!=(const Iterator& it) const noexcept
        {
          return !(*this == it);
        }

        bool operator!=(const ConstIterator& it)
        {
          return !(*this == it);
        }

      private:
        friend BinarySearchTree;
        explicit Iterator(Node* node):
          ptr_(node)
        {}

        Node* ptr_;
      };

      struct ConstIterator: public std::iterator< std::bidirectional_iterator_tag, PairT >
      {
        friend Iterator;

      public:
        ConstIterator& operator++()
        {
          if (!ptr_)
          {
            return *this;
          }

          if (ptr_->right)
          {
            ptr_ = ptr_->right->getMin();
            return *this;
          }

          ComparatorT comparator;
          if (ptr_->up && comparator(ptr_->stored.first, ptr_->up->stored.first))
          {
            ptr_ = ptr_->up;
            return *this;
          }

          while (ptr_->up && comparator(ptr_->up->stored.first, ptr_->stored.first))
          {
            ptr_ = ptr_->up;
          }
          ptr_ = ptr_->up;
          return *this;
        }

        ConstIterator& operator--()
        {
          if (!ptr_)
          {
            return nullptr;
          }

          if (ptr_->left_)
          {
            ptr_ = ptr_->left_->getMax();
            return *this;
          }

          ComparatorT comparator;
          if (ptr_->up && comparator(ptr_->up->stored.first, ptr_->stored.first))
          {
            ptr_ = ptr_->up;
            return *this;
          }

          while (ptr_->up && comparator(ptr_->stored.first, ptr_->up->stored.first))
          {
            ptr_ = ptr_->up;
          }
          ptr_ = ptr_->up;
          return *this;
        }

        const PairT* operator->() const noexcept
        {
          return std::addressof(ptr_->stored);
        }

        const PairT& operator*() const noexcept
        {
          return ptr_->stored;
        }

        bool operator==(const Iterator& it) const noexcept
        {
          return it.ptr_ == this->ptr_;
        }

        bool operator==(const ConstIterator& it) const noexcept
        {
          return it.ptr_ == this->ptr_;
        }

        bool operator!=(const Iterator& it) const noexcept
        {
          return !(*this == it);
        }

        bool operator!=(const ConstIterator& it)
        {
          return !(*this == it);
        }

        const Node* getNode() const noexcept
        {
          return ptr_;
        }

      private:
        friend BinarySearchTree;

        explicit ConstIterator(const Node* node):
          ptr_(node)
        {}

        const Node* ptr_;
      };

      BinarySearchTree() noexcept:
        crown_(nullptr)
      {}

      BinarySearchTree(const BinarySearchTree& instance):
        crown_(instance.crown_ ? new Node{*(instance.crown_), nullptr} : instance.crown_)
      {}

      BinarySearchTree(BinarySearchTree&& instance):
        crown_(instance.crown_)
      {
        instance.crown_ = nullptr;
      }

      ~BinarySearchTree() noexcept
      {
        if (crown_)
        {
          delete crown_;
          crown_ = nullptr;
        }
      }

      void clear() noexcept
      {
        if (crown_)
        {
          delete crown_;
          crown_ = nullptr;
        }
      }

      explicit BinarySearchTree(const std::initializer_list< PairT > initList):
        crown_(nullptr)
      {
        for (auto&& item: initList)
        {
          insert(item.first, item.second);
        }
      }

      ValueT& operator[](const KeyT& key)
      {
        auto it = find(key);
        if (it == getEnd())
        {
          throw std::logic_error(NOT_FOUND_EXCEPTION);
        }
        return it->second;
      }

      const ValueT& operator[](const KeyT& key) const
      {
        auto it = find(key);
        if (it == getEnd())
        {
          throw std::logic_error(NOT_FOUND_EXCEPTION);
        }
        return it->second;
      }

      BinarySearchTree& operator=(const BinarySearchTree& instance)
      {
        if (crown_)
        {
          delete crown_;
        }
        crown_ = instance.crown_ ? new Node{*instance.crown_, nullptr} : nullptr;
        return *this;
      }

      BinarySearchTree& operator=(BinarySearchTree&& instance)
      {
        if (crown_)
        {
          delete crown_;
        }
        crown_ = instance.crown_;
        instance.crown_ = nullptr;
        return *this;
      }

      Iterator insert(const KeyT& key, const ValueT& value)
      {
        if (!crown_)
        {
          crown_ = new Node{{key, value}, nullptr, nullptr, nullptr};
        }
        Node* node = crown_;
        ComparatorT comparator;
        while (node && key != node->stored.first)
        {
          if (comparator(key, node->stored.first))
          {
            if (node->left)
            {
              node = node->left;
            }
            else
            {
              node->left = new Node{{key, value}, node, nullptr, nullptr};
            }
          }
          else
          {
            if (node->stored.first == key)
            {
              throw std::logic_error("BST::insert : item with this key is already in dictionary");
            }
            if (node->right)
            {
              node = node->right;
            }
            else
            {
              node->right = new Node{{key, value}, node, nullptr, nullptr};
            }
          }
        }
        return Iterator{node};
      }

      bool pop(const KeyT& key) noexcept
      {
        Iterator it = find(key);
        if (it != getEnd())
        {
          Node* const ptr = it.ptr_;
          Node* newNode = nullptr;

          if (ptr->right)
          {
            newNode = ptr->right->getMin();
            if (newNode->up->right == newNode)
            {
              newNode->up->right = newNode->right;
            }
            else
            {
              newNode->up->left = newNode->right;
            }
            if (newNode->right)
            {
              newNode->right->up = newNode->up;
            }

            if (ptr->right)
            {
              ptr->right->up = newNode;
            }
            newNode->right = ptr->right;
            ptr->right = nullptr;
            if (ptr->left)
            {
              ptr->left->up = newNode;
              newNode->left = ptr->left;
            }
            newNode->up = ptr->up;
            if (ptr->up)
            {
              if (ptr->up->left == ptr)
              {
                ptr->up->left = newNode;
              }
              else
              {
                ptr->up->right = newNode;
              }
            }
            else
            {
              this->crown_ = newNode;
            }
          }
          else
          {
            if (ptr->up->left == ptr)
            {
              ptr->up->left = ptr->left;
            }
            else
            {
              ptr->up->right = ptr->left;
            }

            if (ptr->left)
            {
              ptr->left->up = ptr->up;
            }
          }

          ptr->left = nullptr;
          delete ptr;
          return true;
        }
        return false;
      }

      Iterator find(const KeyT& key) noexcept
      {
        Node* node = crown_;
        ComparatorT comparator;
        while (node && key != node->stored.first)
        {
          node = comparator(key, node->stored.first) ? node->left : node->right;
        }
        return Iterator{node};
      }

      ConstIterator find(const KeyT& key) const noexcept
      {
        const Node* node = crown_;
        ComparatorT comparator;
        while (node && key != node->stored.first)
        {
          node = comparator(key, node->stored.first) ? node->left : node->right;
        }
        return ConstIterator{node};
      }

      Iterator getEnd() const noexcept
      {
        return Iterator{nullptr};
      }

      Iterator getAlmostEnd() noexcept
      {
        return {crown_->getMax()};
      }

      Iterator getBegin() noexcept
      {
        return Iterator{crown_->getMin()};
      }

      ConstIterator getAlmostEnd() const noexcept
      {
        return ConstIterator{crown_->getMax()};
      }

      ConstIterator getBegin() const noexcept
      {
        return ConstIterator{crown_->getMin()};
      }

      bool isEmpty() const noexcept
      {
        return !crown_;
      }

      size_t getSize() const noexcept
      {
        if (crown_)
        {
          return crown_->getSize();
        }
        return 0;
      }

    protected:
      static Node* open(const Iterator it) noexcept
      {
        return it.ptr_;
      }

      static Node* getCrown(Node* node) noexcept
      {
        while (node->up)
        {
          node = node->up;
        }
        return node;
      }

      Node* crown_;
    };
  }
}

#endif
