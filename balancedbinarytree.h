#ifndef BALANCEDBINARYTREE_H
#define BALANCEDBINARYTREE_H

#include "binarysearchtree.h"

namespace BinaryTreeHelpers
{
    template<class T>
    static void rightRotate(std::shared_ptr<T>& in_root)
    {
        const auto old_root = in_root;
        in_root = in_root->left;
        old_root->left = in_root->right;
        in_root->right = old_root;
    }

    template<class T>
    static void leftRotate(std::shared_ptr<T>& in_root)
    {
        const auto old_root = in_root;
        in_root = in_root->right;
        old_root->right = in_root->left;
        in_root->left = old_root;
    }

    template<class T>
    static int getHeight(const std::shared_ptr<T>& in_root)
    {
        return in_root ? 1 + std::max(getHeight(in_root->left), getHeight(in_root->right)) : -1;
    }

    template<class T>
    static int getBalanceFactor(const std::shared_ptr<T>& in_root)
    {
        return in_root ? getHeight(in_root->left) - getHeight(in_root->right) : -1;
    }
}


template<class T>
class BalancedBinaryTree : public BinarySearchTree<T>
{
public:
    BalancedBinaryTree() {}

    using Super = BinarySearchTree<T>;

protected:
    virtual bool addInternal(const T& value, std::shared_ptr<typename BinaryTreeBase<T>::node>& in_root, const std::shared_ptr<typename BinaryTreeBase<T>::node>& in_parent) override;
    virtual bool removeInternal(const T& value, std::shared_ptr<typename BinaryTreeBase<T>::node>& in_root) override;
};

template<class T>
inline bool BalancedBinaryTree<T>::addInternal(const T &value, std::shared_ptr<typename BinaryTreeBase<T>::node> &in_root, const std::shared_ptr<typename BinaryTreeBase<T>::node> &in_parent)
{
    if(Super::addInternal(value, in_root, in_parent))
    {
        const int balance_factor = BinaryTreeHelpers::getBalanceFactor(in_root);
        if(balance_factor > 1)
        {
            if(value < in_root->left->value)
            {
                BinaryTreeHelpers::rightRotate(in_root);
            }
            else
            {
                BinaryTreeHelpers::leftRotate(in_root->left);
                BinaryTreeHelpers::rightRotate(in_root);
            }
        }
        else if(balance_factor < -1)
        {
            if(value < in_root->right->value)
            {
                BinaryTreeHelpers::rightRotate(in_root->right);
                BinaryTreeHelpers::leftRotate(in_root);
            }
            else
            {
                BinaryTreeHelpers::leftRotate(in_root);
            }
        }
        return true;
    }

    return false;
}

template<class T>
inline bool BalancedBinaryTree<T>::removeInternal(const T &value, std::shared_ptr<typename BinaryTreeBase<T>::node> &in_root)
{
    if(Super::removeInternal(value, in_root))
    {
        const int balance_factor = BinaryTreeHelpers::getBalanceFactor(in_root);
        if(balance_factor == 2)
        {
            if(BinaryTreeHelpers::getBalanceFactor(in_root->right) >= 0)
            {
                BinaryTreeHelpers::rightRotate(in_root);
            }
            else
            {
                BinaryTreeHelpers::leftRotate(in_root->left);
                BinaryTreeHelpers::rightRotate(in_root);
            }
        }
        else if(balance_factor == -2)
        {
            if(BinaryTreeHelpers::getBalanceFactor(in_root->left) <= 0)
            {
                BinaryTreeHelpers::leftRotate(in_root);
            }
            else
            {
                BinaryTreeHelpers::rightRotate(in_root->right);
                BinaryTreeHelpers::leftRotate(in_root);
            }
        }
        return true;
    }

    return false;
}


#endif // BALANCEDBINARYTREE_H
