#ifndef AVL_HPP
#define AVL_HPP

#include "BinarySearchTree.hpp"
#include "Stack.hpp"
#include "Queue.hpp"

namespace malakhov
{
  namespace detail
  {
    template < typename KeyT, typename ValueT, typename ComparatorT = ::std::less< KeyT > >
    class AVLTree: public BinarySearchTree< KeyT, ValueT, ComparatorT >
    {
      using Base = BinarySearchTree< KeyT, ValueT, ComparatorT >;
      using Node = typename Base::Node;

    public:
      using Iterator = typename Base::Iterator;
      using ConstIterator = typename Base::ConstIterator;
      using PairT = typename Base::PairT;

      AVLTree() noexcept:
        Base()
      {}

      AVLTree(const AVLTree& instance):
        Base(instance)
      {}

      AVLTree(AVLTree&& instance):
        Base(instance)
      {}

      AVLTree& operator=(const AVLTree& instance)
      {
        Base::operator=(instance);
        return *this;
      }

      AVLTree& operator=(AVLTree&& instance)
      {
        Base::operator=(instance);
        return *this;
      }

      explicit AVLTree(const std::initializer_list< PairT > initList):
        Base()
      {
        for (auto&& item: initList)
        {
          insert(item.first, item.second);
        }
      }

      Iterator insert(const KeyT& key, const ValueT& value)
      {
        Iterator result = Base::insert(key, value);
        Node* ptr = Base::open(result);
        if (ptr->up)
        {
          balance(*(ptr->up));
        }
        Base::crown_ = Base::getCrown(Base::crown_);
        return result;
      }

      bool pop(const KeyT& key) noexcept
      {
        bool result = Base::pop(key);
        if (result)
        {
          if (Base::crown_->right)
          {
            balance(*(Base::crown_->right->getMin()));
          }
        }
        Base::crown_ = Base::getCrown(Base::crown_);
        return result;
      }

      template< typename Functor >
      Functor traverseLR(Functor f) const
      {
        if (!Base::crown_)
        {
          return f;
        }

        malakhov::Stack< Node* > stack;
        stack.putIn(Base::crown_);
        Node* lastProcessed = nullptr;
        while (!stack.isEmpty())
        {
          Node* toProcess = stack.getLast();
          if (toProcess->left && toProcess->left != lastProcessed)
          {
            do
            {
              toProcess = toProcess->left;
              stack.putIn(toProcess);
            }
            while (toProcess->left);
          }
          f(toProcess->stored);
          lastProcessed = toProcess;
          stack.popOut();
          if (toProcess->right)
          {
            stack.putIn(toProcess->right);
          }
        }
        return f;
      }

      template< typename Functor >
      Functor traverseRL(Functor f) const
      {
        if (!Base::crown_)
        {
          return f;
        }

        malakhov::Stack< Node* > stack;
        stack.putIn(Base::crown_);
        Node* lastProcessed = nullptr;
        while (!stack.isEmpty())
        {
          Node* toProcess = stack.getLast();
          if (toProcess->right && toProcess->right != lastProcessed)
          {
            do
            {
              toProcess = toProcess->right;
              stack.putIn(toProcess);
            }
            while (toProcess->right);
          }
          f(toProcess->stored);
          lastProcessed = toProcess;
          stack.popOut();
          if (toProcess->left)
          {
            stack.putIn(toProcess->left);
          }
        }
        return f;
      }

      template< typename Functor >
      Functor traverseBreadth(Functor f) const
      {
        if (!Base::crown_)
        {
          return f;
        }

        Queue< Node* > queue;
        queue.putIn(Base::crown_);

        while (!queue.isEmpty())
        {
          Node* const first = queue.getFirst();
          queue.popOut();
          f(first->stored);
          if (first->left)
          {
            queue.putIn(first->left);
          }
          if (first->right)
          {
            queue.putIn(first->right);
          }
        }
        return f;
      }
    private:
      static void balance(Node& node) noexcept
      {
        const char balanceFactor = node.getBalanceFactor();
        if (balanceFactor == 2)
        {
          if (node.right->getBalanceFactor() < 0)
          {
            node.right = node.right->rTurn();
          }
          if (node.up)
          {
            if (node.up->left == std::addressof(node))
            {
              node.up->left = node.lTurn();
            }
            else
            {
              node.up->right = node.lTurn();
            }
          }
          else
          {
            node.lTurn();
          }
        }
        else if (balanceFactor == -2)
        {
          if (node.left->getBalanceFactor() > 0)
          {
            node.left = node.left->lTurn();
          }
          if (node.up)
          {
            if (node.up->left == std::addressof(node))
            {
              node.up->left = node.rTurn();
            }
            else
            {
              node.up->right = node.rTurn();
            }
          }
          else
          {
            node.rTurn();
          }
        }
      }
    };
  }
}

#endif
