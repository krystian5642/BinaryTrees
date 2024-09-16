#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "binarysearchtree.h"

template <class ValueType>
class RedBlackTree : public BinarySearchTree<ValueType>
{
public:
    RedBlackTree();

    using BinaryTreeNode = typename BinaryTreeBase<ValueType>::BinaryTreeNode;
    using BinarySearchTreeNode = typename BinarySearchTree<ValueType>::BinarySearchTreeNode;

    virtual shared_ptr<BinaryTreeNode> getLeafNode() const override { return nillNode; }
protected:
    virtual shared_ptr<BinaryTreeNode> removeInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot, bool &removed) override;
    virtual shared_ptr<BinaryTreeNode> createNode(const ValueType &value) const override;

    virtual void postAddInternal(const shared_ptr<BinaryTreeNode> &newNode) override;

    void fixAdd(shared_ptr<BinarySearchTreeNode> node);
    void fixDelete(shared_ptr<BinarySearchTreeNode> node);

protected:
    shared_ptr<BinarySearchTreeNode> nillNode;
};

template <class ValueType>
inline RedBlackTree<ValueType>::RedBlackTree()
{
    nillNode = this->template getNodeAs<BinarySearchTreeNode>(this->createNode(0));
    nillNode->color = QColorConstants::Black;
    this->root = nillNode;
}

template <class ValueType>
shared_ptr<typename RedBlackTree<ValueType>::BinaryTreeNode> RedBlackTree<ValueType>::removeInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot, bool &removed)
{
    const auto nodePtr = this->template getNodeAs<BinarySearchTreeNode>(this->getNodeForValue(value));
    if (nodePtr != this->getLeafNode())
    {
        removed = true;

        shared_ptr<BinarySearchTreeNode> y = nodePtr;
        shared_ptr<BinarySearchTreeNode> x = nullptr;

        QColor color = nodePtr->color;
        if(nodePtr->left == nillNode)
        {
            x = nodePtr->right;
            this->transplant(nodePtr, x);
        }
        else if(nodePtr->right == nillNode)
        {
            x = nodePtr->left;
            this->transplant(nodePtr, x);
        }
        else
        {
            y = this->template getNodeAs<BinarySearchTreeNode>(this->getMinValuePtr(nodePtr->right));
            x = y->right;
            color = y->color;
            if(y->getParent() == nodePtr)
            {
                x->parent = y;
            }
            else
            {
                this->transplant(y, x);
                y->right = nodePtr->right;
                y->right->parent = y;
            }

            this->transplant(nodePtr, y);
            y->left = nodePtr->left;
            y->left->parent = y;
            y->color = nodePtr->color;
        }

        if(color == QColorConstants::Black)
        {
            this->fixDelete(x);
        }

        return this->root;
    }

    removed = false;
    return this->root;
}

template <class ValueType>
inline shared_ptr<typename RedBlackTree<ValueType>::BinaryTreeNode> RedBlackTree<ValueType>::createNode(const ValueType &value) const
{
    const auto newNode = std::make_shared<BinarySearchTreeNode>(value);
    newNode->left = nillNode;
    newNode->right = nillNode;
    newNode->color = QColorConstants::Red;
    return newNode;
}

template <class ValueType>
inline void RedBlackTree<ValueType>::postAddInternal(const shared_ptr<BinaryTreeNode> &newNode)
{
    this->fixAdd(this->template getNodeAs<BinarySearchTreeNode>(newNode));
}

template<class ValueType>
inline void RedBlackTree<ValueType>::fixAdd(shared_ptr<BinarySearchTreeNode> node)
{
    if (node == this->getLeafNode())
    {
        return;
    }

    if (node == this->root)
    {
        node->color = QColorConstants::Black;
        return;
    }

    auto parent = this->template getNodeAs<BinarySearchTreeNode>(node->getParent());
    while(node != this->root && parent->color == QColorConstants::Red)
    {
        const auto grandparent = this->template getNodeAs<BinarySearchTreeNode>(parent->getParent());
        if(parent == grandparent->left)
        {
            const auto uncle = grandparent->right;
            if(uncle && uncle->color == QColorConstants::Red)
            {
                if(parent->right == node)
                {
                    this->leftRotate(parent);

                    node->color = QColorConstants::Black;
                }
                else
                {
                    parent->color = QColorConstants::Black;
                    node->color = QColorConstants::Red;
                }

                if(grandparent != this->root)
                {
                    grandparent->color = QColorConstants::Red;
                }
                uncle->color = QColorConstants::Black;
            }
            else
            {
                if(parent->right == node)
                {
                    this->leftRotate(parent);
                    this->rightRotate(grandparent);

                    node->color = QColorConstants::Black;
                }
                else if(parent->left == node)
                {
                    this->rightRotate(grandparent);

                    parent->color = QColorConstants::Black;
                }

                if(grandparent != this->root)
                {
                    grandparent->color = QColorConstants::Red;
                }
            }

            node = grandparent;
        }
        else if(parent == grandparent->right)
        {
            const auto uncle = grandparent->left;
            if(uncle && uncle->color == QColorConstants::Red)
            {
                if(parent->left == node)
                {
                    this->rightRotate(parent);

                    node->color = QColorConstants::Black;
                }
                else
                {
                    parent->color = QColorConstants::Black;
                    node->color = QColorConstants::Red;
                }

                if(grandparent != this->root)
                {
                    grandparent->color = QColorConstants::Red;
                }

                uncle->color = QColorConstants::Black;
            }
            else
            {
                if(parent->left == node)
                {
                    this->rightRotate(parent);
                    this->leftRotate(grandparent);

                    node->color = QColorConstants::Black;
                }
                else if(parent->right == node)
                {
                    this->leftRotate(grandparent);

                    parent->color = QColorConstants::Black;
                }

                if(grandparent != this->root)
                {
                    grandparent->color = QColorConstants::Red;
                }
            }
        }

        parent = this->template getNodeAs<BinarySearchTreeNode>(node->getParent());
    }
}

template<class ValueType>
inline void RedBlackTree<ValueType>::fixDelete(shared_ptr<BinarySearchTreeNode> node)
{
    shared_ptr<BinarySearchTreeNode> sibling = nullptr;
    auto parent = this->template getNodeAs<BinarySearchTreeNode>(node->getParent());
    while (node != this->root && node->color == QColorConstants::Black)
    {
        if (node == parent->left)
        {
            sibling = parent->right;
            if (sibling->color == QColorConstants::Red)
            {
                sibling->color = QColorConstants::Black;
                parent->color = QColorConstants::Red;
                this->leftRotate(parent);
                sibling = parent->right;
            }

            if (sibling->left->color == QColorConstants::Black && sibling->right->color == QColorConstants::Black)
            {
                sibling->color = QColorConstants::Red;
                node = parent;
            }
            else
            {
                if (sibling->right->color == QColorConstants::Black)
                {
                    sibling->color = QColorConstants::Red;
                    sibling->left->color = QColorConstants::Black;
                    this->rightRotate(sibling);
                    sibling = parent->right;
                }

                sibling->color = parent->color;
                parent->color = QColorConstants::Black;
                sibling->right->color = QColorConstants::Black;
                this->leftRotate(parent);
                node = this->template getNodeAs<BinarySearchTreeNode>(this->root);
            }
        }
        else
        {
            sibling = parent->left;
            if (sibling->color == QColorConstants::Red)
            {
                sibling->color = QColorConstants::Black;
                parent->color = QColorConstants::Red;
                this->rightRotate(parent);
                sibling = parent->left;
            }

            if (sibling->left->color == QColorConstants::Black && sibling->right->color == QColorConstants::Black)
            {
                sibling->color = QColorConstants::Red;
                node = parent;
            }
            else
            {
                if (sibling->left->color == QColorConstants::Black)
                {
                    sibling->color = QColorConstants::Red;
                    sibling->right->color = QColorConstants::Black;
                    this->leftRotate(sibling);
                    sibling = parent->left;
                }

                sibling->color = parent->color;
                parent->color = QColorConstants::Black;
                sibling->left->color = QColorConstants::Black;
                this->rightRotate(parent);
                node = this->template getNodeAs<BinarySearchTreeNode>(this->root);
            }
        }

        auto parent = this->template getNodeAs<BinarySearchTreeNode>(node->getParent());
    }
    node->color = QColorConstants::Black;
}

#endif // REDBLACKTREE_H
