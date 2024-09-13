#ifndef BALANCEDBINARYTREE_H
#define BALANCEDBINARYTREE_H

#include "binarysearchtree.h"

template <class ValueType>
class BalancedBinaryTree : public BinarySearchTree<ValueType>
{
public:
    BalancedBinaryTree() = default;

    using Node = typename BinaryTreeBase<ValueType>::Node;

protected:
    virtual void postAddInternal(const shared_ptr<Node> &newNode) override;
    virtual void postRemoveInternal() override;

    void fixRotations(const shared_ptr<Node> &inRoot);
};

template <class ValueType>
inline void BalancedBinaryTree<ValueType>::postAddInternal(const shared_ptr<Node> &newNode)
{
    fixRotations(this->root);
}

template <class ValueType>
inline void BalancedBinaryTree<ValueType>::postRemoveInternal()
{
    fixRotations(this->root);
}

template <class ValueType>
inline void BalancedBinaryTree<ValueType>::fixRotations(const shared_ptr<Node> &inRoot)
{
    if (inRoot.get() == this->getLeafNode())
    {
        return;
    }

    fixRotations(inRoot->left);

    const int balanceFactor = this->getBalanceFactor(inRoot);
    if (balanceFactor == 2)
    {
        if (this->getBalanceFactor(inRoot->right) >= 0)
        {
            this->rightRotate(inRoot);
        }
        else
        {
            this->leftRotate(inRoot->left);
            this->rightRotate(inRoot);
        }
    }
    else if (balanceFactor == -2)
    {
        if (this->getBalanceFactor(inRoot->left) <= 0)
        {
            this->leftRotate(inRoot);
        }
        else
        {
            this->rightRotate(inRoot->right);
            this->leftRotate(inRoot);
        }
    }

    fixRotations(inRoot->right);
}

#endif // BALANCEDBINARYTREE_H
