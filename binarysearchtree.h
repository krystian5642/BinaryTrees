#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "binarytreebase.h"

template <class ValueType>
class BinarySearchTree : public BinaryTreeBase<ValueType>
{
public:
    BinarySearchTree() = default;

    using Node = typename BinaryTreeBase<ValueType>::Node;

protected:
    virtual shared_ptr<Node> addInternal(const ValueType &value, const shared_ptr<Node> &inRoot, const shared_ptr<Node> &removed, shared_ptr<Node> &newNode) override;
    virtual shared_ptr<Node> removeInternal(const ValueType &value, const shared_ptr<Node> &valuePtr, bool &removed) override;
    virtual shared_ptr<Node> getNodeForValue(const ValueType &value) const override;

    shared_ptr<Node> getNodeForValueInternal(const ValueType &value, const shared_ptr<Node> &inRoot) const;

    void rightRotate(const shared_ptr<Node> &inRoot);
    void leftRotate(const shared_ptr<Node> &inRoot);

    int getHeight(const shared_ptr<Node> &inRoot);
    int getBalanceFactor(const shared_ptr<Node> &inRoot);
};

template <class ValueType>
shared_ptr<typename BinaryTreeBase<ValueType>::Node> BinarySearchTree<ValueType>::addInternal(const ValueType &value, const shared_ptr<Node> &inRoot
                                                                                                            , const shared_ptr<Node> &parent, shared_ptr<Node> &newNode)
{
    if(inRoot == this->getLeafNode())
    {
        newNode = this->createNode(value);
        newNode->parent = parent;
        return newNode;
    }

    if(inRoot->value < value)
    {
        inRoot->right = addInternal(value, inRoot->right, inRoot, newNode);
    }
    else if (inRoot->value > value)
    {
        inRoot->left = addInternal(value, inRoot->left, inRoot, newNode);
    }
    else
    {
        newNode = nullptr;
    }

    return inRoot;
}

template <class ValueType>
shared_ptr<typename BinaryTreeBase<ValueType>::Node> BinarySearchTree<ValueType>::removeInternal(const ValueType &value, const shared_ptr<Node> &valuePtr, bool &removed)
{
    if (valuePtr == this->getLeafNode())
    {
        removed = false;
    }
    else if (valuePtr->value < value)
    {
        valuePtr->right = removeInternal(value, valuePtr->right, removed);
    }
    else if (valuePtr->value > value)
    {
        valuePtr->left = removeInternal(value, valuePtr->left, removed);
    }
    else
    {
        removed = true;
    }

    // Node with only right child or no child
    if (!valuePtr->left)
    {
        return valuePtr->right;
    }

    // Node with only left child
    if (!valuePtr->right)
    {
        return valuePtr->left;
    }

    // Node with 2 children
    const auto leftMax = this->getMaxValuePtr(valuePtr->left);
    valuePtr->value = leftMax->value;
    if(leftMax->getParentAsSharedPtr()->left == leftMax)
    {
        leftMax->getParentAsSharedPtr()->left = nullptr;
    }
    else
    {
        leftMax->getParentAsSharedPtr()->right = nullptr;
    }

    return valuePtr;
}

template <class ValueType>
inline shared_ptr<typename BinaryTreeBase<ValueType>::Node> BinarySearchTree<ValueType>::getNodeForValue(const ValueType &value) const
{
    return getNodeForValueInternal(value, this->root);
}

template <class ValueType>
inline shared_ptr<typename BinaryTreeBase<ValueType>::Node> BinarySearchTree<ValueType>::getNodeForValueInternal(const ValueType &value, const shared_ptr<Node> &inRoot) const
{
    if (inRoot == this->getLeafNode())
    {
        return inRoot;
    }

    if (inRoot->value < value)
    {
        return getNodeForValueInternal(value, inRoot->right);
    }

    if (inRoot->value > value)
    {
        return getNodeForValueInternal(value, inRoot->left);
    }

    return inRoot;
}

template <class ValueType>
inline void BinarySearchTree<ValueType>::rightRotate(const shared_ptr<Node> &inRoot)
{
    const auto newRoot = inRoot->left;
    inRoot->left = newRoot->right;
    newRoot->right = inRoot;
    if (const auto parent = inRoot->getParentAsSharedPtr())
    {
        if (parent->left == inRoot)
        {
            parent->left = newRoot;
        }
        else
        {
            parent->right = newRoot;
        }
    }
    else
    {
        this->root = newRoot;
    }

    // update parent
    newRoot->parent = inRoot->parent;
    inRoot->parent = newRoot;
    if(inRoot->left != this->getLeafNode())
    {
        inRoot->left->parent = inRoot;
    }
}

template <class ValueType>
inline void BinarySearchTree<ValueType>::leftRotate(const shared_ptr<Node> &inRoot)
{
    const auto newRoot = inRoot->right;
    inRoot->right = newRoot->left;
    newRoot->left = inRoot; 
    if (const auto parent = inRoot->getParentAsSharedPtr())
    {
        if (parent->left == inRoot)
        {
            parent->left = newRoot;
        }
        else
        {
            parent->right = newRoot;
        }
    }
    else
    {
        this->root = newRoot;
    }

    // update parent
    newRoot->parent = inRoot->parent;
    inRoot->parent = newRoot;
    if (inRoot->right != this->getLeafNode())
    {
        inRoot->right->parent = inRoot;
    }
}

template <class ValueType>
inline int BinarySearchTree<ValueType>::getHeight(const shared_ptr<Node> &inRoot)
{
    return inRoot != this->getLeafNode() ? 1 + max(getHeight(inRoot->left), getHeight(inRoot->right)) : -1;
}

template <class ValueType>
inline int BinarySearchTree<ValueType>::getBalanceFactor(const shared_ptr<Node> &inRoot)
{
    return inRoot != this->getLeafNode() ? getHeight(inRoot->left) - getHeight(inRoot->right) : -1;
}

#endif // BINARYSEARCHTREE_H
