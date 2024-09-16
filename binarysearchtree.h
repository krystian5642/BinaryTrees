#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "binarytreebase.h"

template <class ValueType>
class BinarySearchTree : public BinaryTreeBase<ValueType>
{
public:
    using BinaryTreeNode = typename BinaryTreeBase<ValueType>::BinaryTreeNode;

    struct BinarySearchTreeNode : public BinaryTreeNode
    {
        BinarySearchTreeNode(const ValueType &value)
            : BinaryTreeNode(value)
        {}

        virtual shared_ptr<BinaryTreeNode> getParent() const override { return parent.lock(); }
        virtual shared_ptr<BinaryTreeNode> getLeft() const override { return left; }
        virtual shared_ptr<BinaryTreeNode> getRight() const override { return right; }

        weak_ptr<BinarySearchTreeNode> parent;
        shared_ptr<BinarySearchTreeNode> left;
        shared_ptr<BinarySearchTreeNode> right;
    };

protected:
    virtual shared_ptr<BinaryTreeNode> addInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot, const shared_ptr<BinaryTreeNode> &removed, shared_ptr<BinaryTreeNode> &newNode) override;
    virtual shared_ptr<BinaryTreeNode> removeInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot, bool &removed) override;
    virtual shared_ptr<BinaryTreeNode> getMaxValuePtr(const shared_ptr<BinaryTreeNode> &inRoot) const override;
    virtual shared_ptr<BinaryTreeNode> getMinValuePtr(const shared_ptr<BinaryTreeNode> &inRoot) const override;

    virtual shared_ptr<BinaryTreeNode> getNodeForValue(const ValueType &value) const override;
    shared_ptr<BinaryTreeNode> getNodeForValueInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot) const;

    void rightRotate(const shared_ptr<BinarySearchTreeNode> &inRoot);
    void leftRotate(const shared_ptr<BinarySearchTreeNode> &inRoot);

    int getHeight(const shared_ptr<BinarySearchTreeNode> &inRoot);
    int getBalanceFactor(const shared_ptr<BinarySearchTreeNode> &inRoot);

    void transplant(const shared_ptr<BinarySearchTreeNode> &u, const shared_ptr<BinarySearchTreeNode> &v);
};

template <class ValueType>
shared_ptr<typename BinarySearchTree<ValueType>::BinaryTreeNode> BinarySearchTree<ValueType>::addInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot
                                                                                                          , const shared_ptr<BinaryTreeNode> &parent, shared_ptr<BinaryTreeNode> &newNode)
{
    if(inRoot == this->getLeafNode())
    {
        const auto newNodePtr = this->template getNodeAs<BinarySearchTreeNode>(this->createNode(value));
        newNodePtr->parent = this->template getNodeAs<BinarySearchTreeNode>(parent);
        newNode = newNodePtr;
        return newNode;
    }

    const auto binarySearchTreeRoot = this->template getNodeAs<BinarySearchTreeNode>(inRoot);
    if(binarySearchTreeRoot->value < value)
    {
        binarySearchTreeRoot->right = this->template getNodeAs<BinarySearchTreeNode>(addInternal(value, binarySearchTreeRoot->right, inRoot, newNode));
    }
    else if (binarySearchTreeRoot->value > value)
    {
        binarySearchTreeRoot->left = this->template getNodeAs<BinarySearchTreeNode>(addInternal(value, binarySearchTreeRoot->left, inRoot, newNode));
    }
    else
    {
        newNode = this->getLeafNode();
    }

    return binarySearchTreeRoot;
}

template <class ValueType>
shared_ptr<typename BinarySearchTree<ValueType>::BinaryTreeNode> BinarySearchTree<ValueType>::removeInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot, bool &removed)
{
    if (inRoot == this->getLeafNode())
    {
        removed = false;
        return inRoot;
    }

    const auto binarySearchTreeRoot = this->template getNodeAs<BinarySearchTreeNode>(inRoot);
    if (binarySearchTreeRoot->value < value)
    {
        binarySearchTreeRoot->right = this->template getNodeAs<BinarySearchTreeNode>(removeInternal(value, binarySearchTreeRoot->right, removed));
    }
    else if (binarySearchTreeRoot->value > value)
    {
        binarySearchTreeRoot->left = this->template getNodeAs<BinarySearchTreeNode>(removeInternal(value, binarySearchTreeRoot->left, removed));
    }
    else
    {
        removed = true;
    }

    // Node with only right child or no child
    if (binarySearchTreeRoot->left == this->getLeafNode())
    {
        return binarySearchTreeRoot->right;
    }

    // Node with only left child
    if (binarySearchTreeRoot->right == this->getLeafNode())
    {
        return binarySearchTreeRoot->left;
    }

    // Node with 2 children
    const auto leftMax = this->template getNodeAs<BinarySearchTreeNode>(this->getMaxValuePtr(binarySearchTreeRoot->left));
    binarySearchTreeRoot->value = leftMax->value;
    this->transplant(leftMax, nullptr);

    return binarySearchTreeRoot;
}

template<class ValueType>
inline shared_ptr<typename BinaryTreeBase<ValueType>::BinaryTreeNode> BinarySearchTree<ValueType>::getMaxValuePtr(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    const auto binarySearchTreeRoot = this->template getNodeAs<BinarySearchTreeNode>(inRoot);
    return binarySearchTreeRoot != this->getLeafNode() && binarySearchTreeRoot->right != this->getLeafNode() ? getMaxValuePtr(binarySearchTreeRoot->right) : binarySearchTreeRoot;
}

template<class ValueType>
inline shared_ptr<typename BinaryTreeBase<ValueType>::BinaryTreeNode> BinarySearchTree<ValueType>::getMinValuePtr(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    const auto binarySearchTreeRoot = this->template getNodeAs<BinarySearchTreeNode>(inRoot);
    return binarySearchTreeRoot != this->getLeafNode() && binarySearchTreeRoot->left != this->getLeafNode() ? getMinValuePtr(binarySearchTreeRoot->left) : binarySearchTreeRoot;
}

template <class ValueType>
inline shared_ptr<typename BinaryTreeBase<ValueType>::BinaryTreeNode> BinarySearchTree<ValueType>::getNodeForValue(const ValueType &value) const
{
    return getNodeForValueInternal(value, this->root);
}

template <class ValueType>
inline shared_ptr<typename BinaryTreeBase<ValueType>::BinaryTreeNode> BinarySearchTree<ValueType>::getNodeForValueInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot) const
{
    if (inRoot == this->getLeafNode())
    {
        return inRoot;
    }

    if (inRoot->value < value)
    {
        return getNodeForValueInternal(value, this->template getNodeAs<BinarySearchTreeNode>(inRoot)->right);
    }

    if (inRoot->value > value)
    {
        return getNodeForValueInternal(value, this->template getNodeAs<BinarySearchTreeNode>(inRoot)->left);
    }

    return inRoot;
}

template <class ValueType>
inline void BinarySearchTree<ValueType>::rightRotate(const shared_ptr<BinarySearchTreeNode> &inRoot)
{
    const auto oldRoot = this->template getNodeAs<BinarySearchTreeNode>(inRoot);
    const auto newRoot = oldRoot->left;
    oldRoot->left = newRoot->right;
    newRoot->right = oldRoot;
    this->transplant(oldRoot, newRoot);

    // update parent
    oldRoot->parent = newRoot;
    if(oldRoot->left != this->getLeafNode())
    {
        oldRoot->left->parent = oldRoot;
    }
}

template <class ValueType>
inline void BinarySearchTree<ValueType>::leftRotate(const shared_ptr<BinarySearchTreeNode> &inRoot)
{
    const auto oldRoot = this->template getNodeAs<BinarySearchTreeNode>(inRoot);
    const auto newRoot = oldRoot->right;
    oldRoot->right = newRoot->left;
    newRoot->left = oldRoot;
    this->transplant(oldRoot, newRoot);

    // update parent
    oldRoot->parent = newRoot;
    if (oldRoot->right != this->getLeafNode())
    {
        oldRoot->right->parent = oldRoot;
    }
}

template <class ValueType>
inline int BinarySearchTree<ValueType>::getHeight(const shared_ptr<BinarySearchTreeNode> &inRoot)
{
    const auto binarySearchTreeRoot = this->template getNodeAs<BinarySearchTreeNode>(inRoot);
    return binarySearchTreeRoot != this->getLeafNode() ? 1 + max(getHeight(binarySearchTreeRoot->left), getHeight(binarySearchTreeRoot->right)) : -1;
}

template <class ValueType>
inline int BinarySearchTree<ValueType>::getBalanceFactor(const shared_ptr<BinarySearchTreeNode> &inRoot)
{
    const auto binarySearchTreeRoot = this->template getNodeAs<BinarySearchTreeNode>(inRoot);
    return binarySearchTreeRoot != this->getLeafNode() ? getHeight(binarySearchTreeRoot->left) - getHeight(binarySearchTreeRoot->right) : -1;
}

template<class ValueType>
inline void BinarySearchTree<ValueType>::transplant(const shared_ptr<BinarySearchTreeNode> &u, const shared_ptr<BinarySearchTreeNode> &v)
{
    const auto parent = this->template getNodeAs<BinarySearchTreeNode>(u->getParent());
    if (!parent)
    {
        this->root = v;
    }
    else if (parent->left == u)
    {
        parent->left = v;
    }
    else
    {
        parent->right = v;
    }

    if (v)
    {
        v->parent = parent;
    }
}

#endif // BINARYSEARCHTREE_H
