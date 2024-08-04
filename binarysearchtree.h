#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "binarytreebase.h"

template<class T>
class BinarySearchTree : public BinaryTreeBase<T>
{
public:
    BinarySearchTree() {}

protected:
    virtual bool addInternal(const T& value, std::shared_ptr<typename BinaryTreeBase<T>::node>& in_root, const std::shared_ptr<typename BinaryTreeBase<T>::node>& in_parent) override;
    virtual bool removeInternal(const T& value, std::shared_ptr<typename BinaryTreeBase<T>::node>& in_root) override;
};

template<class T>
inline bool BinarySearchTree<T>::addInternal(const T &value, std::shared_ptr<typename BinaryTreeBase<T>::node> &in_root, const std::shared_ptr<typename BinaryTreeBase<T>::node> &in_parent)
{
    if(!in_root)
    {
        in_root = std::make_shared<typename BinaryTreeBase<T>::node>(value);
        in_root->parent = in_parent;
        return true;
    }

    if(in_root->value < value)
    {
        return addInternal(value, in_root->right, in_root);
    }
    else if(in_root->value > value)
    {
        return addInternal(value, in_root->left, in_root);
    }

    return true;
}

template<class T>
inline bool BinarySearchTree<T>::removeInternal(const T &value, std::shared_ptr<typename BinaryTreeBase<T>::node> &in_root)
{
    if(!in_root)
    {
        return false;
    }

    if(in_root->value < value)
    {
        return removeInternal(value, in_root->right);
    }
    else if(in_root->value > value)
    {
        return removeInternal(value, in_root->left);
    }
    else
    {
        // Node with only right child or no child
        if(!in_root->left)
        {
            in_root = in_root->right;
            return true;
        }

        // Node with only left child
        if(!in_root->right)
        {
            in_root = in_root->left;
            return true;
        }

        // Node with 2 children
        auto& left_max = this->getMaxValuePtr(in_root->left);
        in_root->value = left_max->value;
        left_max.reset();
    }

    return true;
}

#endif // BINARYSEARCHTREE_H
