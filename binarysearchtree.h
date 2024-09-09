#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "binarytreebase.h"

template<class T>
class BinarySearchTree : public BinaryTreeBase<T>
{
public:
    BinarySearchTree() {}

    using node = typename BinaryTreeBase<T>::node;

protected:
    virtual shared_ptr<node> addInternal(const T &value, const shared_ptr<node>& inRoot, const shared_ptr<node>& parent, shared_ptr<node>& new_node) override;
    virtual shared_ptr<node> removeInternal(const T &value, const shared_ptr<node>& valuePtr, bool& removed) override;
    virtual shared_ptr<node> getNodeForValue(const T &value) const;

    shared_ptr<node> getNodeForValueInternal(const T &value, const shared_ptr<node>& inRoot) const;

    void rotateRight(const shared_ptr<node>& inRoot);
    void rotateLeft(const shared_ptr<node>& inRoot);

    int getHeight(const shared_ptr<node>& inRoot);
    int getBalanceFactor(const shared_ptr<node>& inRoot);
};

template<class T>
inline shared_ptr<typename BinaryTreeBase<T>::node> BinarySearchTree<T>::addInternal(const T &value, const shared_ptr<node>& inRoot, const shared_ptr<node>& parent, shared_ptr<node>& new_node)
{
    if(!inRoot)
    {
        new_node = this->createNode(value);
        new_node->parent = parent;
        return new_node;
    }

    if(inRoot->value < value)
    {
        inRoot->right = BinarySearchTree<T>::addInternal(value, inRoot->right, inRoot, new_node);
    }
    else if(inRoot->value > value)
    {
        inRoot->left = BinarySearchTree<T>::addInternal(value, inRoot->left, inRoot, new_node);
    }
    else
    {
        new_node = nullptr;
    }

    return inRoot;
}

template<class T>
inline shared_ptr<typename BinaryTreeBase<T>::node> BinarySearchTree<T>::removeInternal(const T &value, const shared_ptr<node>& valuePtr, bool& removed)
{
    if(!valuePtr)
    {
        removed = false;
    }
    else if(valuePtr->value < value)
    {
        valuePtr->right = removeInternal(value, valuePtr->right, removed);
    } 
    else if(valuePtr->value > value)
    {
        valuePtr->left = removeInternal(value, valuePtr->left, removed);
    }
    else
    {
        removed = true;

        // Node with only right child or no child
        if(!valuePtr->left)
        {
            return valuePtr->right;
        }

        // Node with only left child
        if(!valuePtr->right)
        {
            return valuePtr->left;
        }

        // Node with 2 children
        auto& left_max = this->getMaxValuePtr(valuePtr->left);
        valuePtr->value = left_max->value;
        left_max.reset();
    }

    return valuePtr;
}

template<class T>
inline shared_ptr<typename BinaryTreeBase<T>::node> BinarySearchTree<T>::getNodeForValue(const T &value) const
{
    return getNodeForValueInternal(value, this->root);
}

template<class T>
inline shared_ptr<typename BinaryTreeBase<T>::node> BinarySearchTree<T>::getNodeForValueInternal(const T &value, const shared_ptr<node> &inRoot) const
{
    if(!inRoot)
    {
        return inRoot;
    }

    if(inRoot->value < value)
    {
        return getNodeForValueInternal(value, inRoot->right);
    }
    else if(inRoot->value > value)
    {
        return getNodeForValueInternal(value, inRoot->left);
    }
    return inRoot;
}

template<class T>
inline void BinarySearchTree<T>::rotateRight(const shared_ptr<typename BinaryTreeBase<T>::node> &inRoot)
{
    const auto new_root = inRoot->left;
    const auto y = new_root->right;
    inRoot->left = y;
    new_root->right = inRoot;
    if(const auto parent = inRoot->parent.lock())
    {
        if(parent->left == inRoot)
        {
            parent->left = new_root;
        }
        else
        {
            parent->right = new_root;
        }
    }
    else
    {
        this->root = new_root;
    }

    //update parent
    new_root->parent = inRoot->parent;
    inRoot->parent = new_root;
    if(inRoot->left)
    {
        inRoot->left->parent = inRoot;
    }
}

template<class T>
inline void BinarySearchTree<T>::rotateLeft(const shared_ptr<typename BinaryTreeBase<T>::node> &inRoot)
{
    const auto new_root = inRoot->right;
    const auto y = new_root->left;
    inRoot->right = y;
    new_root->left = inRoot;
    if(const auto parent = inRoot->parent.lock())
    {
        if(parent->left == inRoot)
        {
            parent->left = new_root;
        }
        else
        {
            parent->right = new_root;
        }
    }
    else
    {
        this->root = new_root;
    }

    //update parent
    new_root->parent = inRoot->parent;
    inRoot->parent = new_root;
    if(inRoot->right)
    {
        inRoot->right->parent = inRoot;
    }
}

template<class T>
inline int BinarySearchTree<T>::getHeight(const shared_ptr<node> &inRoot)
{
    return inRoot ? 1 + max(getHeight(inRoot->left), getHeight(inRoot->right)) : -1;
}

template<class T>
inline int BinarySearchTree<T>::getBalanceFactor(const shared_ptr<node> &inRoot)
{
    return inRoot ? getHeight(inRoot->left) - getHeight(inRoot->right) : -1;
}

#endif // BINARYSEARCHTREE_H








