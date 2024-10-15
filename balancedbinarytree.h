#ifndef BALANCEDBINARYTREE_H
#define BALANCEDBINARYTREE_H

#include "binarysearchtree.h"

template <class ValueType>
class BalancedBinaryTree : public BinarySearchTree<ValueType>
{
public:
    BalancedBinaryTree() = default;

    using BinaryTreeNode = typename BinaryTreeBase<ValueType>::BinaryTreeNode;
    using BinarySearchTreeNode = typename BinarySearchTree<ValueType>::BinarySearchTreeNode;

protected:
    virtual void postAddInternal(const shared_ptr<BinaryTreeNode> &newNode) override;
    virtual void postRemoveInternal() override;

    void fixRotations(const shared_ptr<BinaryTreeNode> &inRoot);
};

template <class ValueType>
inline void BalancedBinaryTree<ValueType>::postAddInternal(const shared_ptr<BinaryTreeNode> &newNode)
{
    this->fixRotations(this->template getNodeAs<BinarySearchTreeNode>(this->root));
}

template <class ValueType>
inline void BalancedBinaryTree<ValueType>::postRemoveInternal()
{
    this->fixRotations(this->template getNodeAs<BinarySearchTreeNode>(this->root));
}

template <class ValueType>
inline void BalancedBinaryTree<ValueType>::fixRotations(const shared_ptr<BinaryTreeNode> &inRoot)
{
    if (!this->isNodeValid(inRoot))
    {
        return;
    }

    const auto binarySearchTreeNode = this->template getNodeAs<BinarySearchTreeNode>(inRoot);
    this->fixRotations(binarySearchTreeNode->left);

    const int balanceFactor = this->getBalanceFactor(binarySearchTreeNode);
    if (balanceFactor == 2)
    {
        if (this->getBalanceFactor(binarySearchTreeNode->right) >= -1)
        {
            this->rightRotate(binarySearchTreeNode);
        }
        else
        {
            this->leftRotate(binarySearchTreeNode->left);
            this->rightRotate(binarySearchTreeNode);
        }
    }
    else if (balanceFactor == -2)
    {
        if (this->getBalanceFactor(binarySearchTreeNode->left) <= 0)
        {
            this->leftRotate(binarySearchTreeNode);
        }
        else
        {
            this->rightRotate(binarySearchTreeNode->right);
            this->leftRotate(binarySearchTreeNode);
        }
    }

    this->fixRotations(binarySearchTreeNode->right);
}

#endif // BALANCEDBINARYTREE_H
