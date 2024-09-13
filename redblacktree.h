#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "binarysearchtree.h"

template <class ValueType>
class RedBlackTree : public BinarySearchTree<ValueType>
{
public:
    RedBlackTree();

    using Node = typename BinaryTreeBase<ValueType>::Node;

    virtual shared_ptr<Node> getLeafNode() const { return nillNode; }
protected:
    virtual void postAddInternal(const shared_ptr<Node> &newNode) override;
    virtual shared_ptr<Node> removeInternal(const ValueType &value, const shared_ptr<Node> &valuePtr, bool &removed) override;
    virtual shared_ptr<Node> createNode(const ValueType &value) const override;

    void transplant(const shared_ptr<Node> &u, const shared_ptr<Node> &v);
    void balanceDelete(shared_ptr<Node> x);

protected:
    shared_ptr<Node> nillNode;
};

template <class ValueType>
inline RedBlackTree<ValueType>::RedBlackTree()
{
    nillNode = this->createNode(0);
    nillNode->color = QColorConstants::Black;
    this->root = nillNode;
}

template <class ValueType>
inline void RedBlackTree<ValueType>::postAddInternal(const shared_ptr<Node> &newNode)
{
    if (newNode == this->getLeafNode())
    {
        return;
    }

    shared_ptr<Node> node = newNode;
    if (node == this->root)
    {
        node->color = QColorConstants::Black;
        return;
    }

    shared_ptr<Node> parent = node->getParentAsSharedPtr();
    while(node != this->root && parent->color == QColorConstants::Red)
    {
        const shared_ptr<Node> grandparent = parent->getParentAsSharedPtr();
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

                if(grandparent!=this->root)
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
            const shared_ptr<Node> uncle = grandparent->left;
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

        parent = node->getParentAsSharedPtr();
    }
}

template <class ValueType>
inline shared_ptr<typename BinaryTreeBase<ValueType>::Node> RedBlackTree<ValueType>::removeInternal(const ValueType &value, const shared_ptr<Node> &valuePtr, bool &removed)
{
    const auto z = this->getNodeForValue(value);
    if (z != this->getLeafNode())
    {
        removed = true;

        shared_ptr<Node> y = z;
        shared_ptr<Node> x = nullptr;

        QColor color = z->color;
        if(z->left == nillNode)
        {
            x = z->right;
            transplant(z,x);
        }
        else if(z->right == nillNode)
        {
            x = z->left;
            transplant(z,x);
        }
        else
        {
            y = this->getMinValuePtr(z->right);
            x = y->right;
            color = y->color;
            if(y->getParentAsSharedPtr() == z)
            {
                x->parent = y;
            }
            else
            {
                transplant(y, x);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        if(color == QColorConstants::Black)
        {
            balanceDelete(x);
        }

        return this->root;
    }

    removed = false;
    return this->root;
}

template <class ValueType>
inline shared_ptr<typename BinaryTreeBase<ValueType>::Node> RedBlackTree<ValueType>::createNode(const ValueType &value) const
{
    const auto newNode = std::make_shared<Node>(value);
    newNode->left = nillNode;
    newNode->right = nillNode;
    newNode->color = QColorConstants::Red;
    return newNode;
}

template <class ValueType>
inline void RedBlackTree<ValueType>::transplant(const shared_ptr<Node> &u, const shared_ptr<Node> &v)
{
    if (u == this->root)
    {
        this->root = v;
    }
    else if (u->getParentAsSharedPtr()->left == u)
    {
        u->getParentAsSharedPtr()->left = v;
    }
    else
    {
        u->getParentAsSharedPtr()->right = v;
    }

    if (v)
    {
        v->parent = u->parent;
    }
}

template <class ValueType>
inline void RedBlackTree<ValueType>::balanceDelete(shared_ptr<Node> node)
{
    shared_ptr<Node> sibling = nullptr;
    while (node != this->root && node->color == QColorConstants::Black)
    {
        if (node == node->getParentAsSharedPtr()->left)
        {
            sibling = node->getParentAsSharedPtr()->right;
            if (sibling->color == QColorConstants::Red)
            {
                sibling->color = QColorConstants::Black;
                node->getParentAsSharedPtr()->color = QColorConstants::Red;
                this->leftRotate(node->getParentAsSharedPtr());
                sibling = node->getParentAsSharedPtr()->right;
            }

            if (sibling->left->color == QColorConstants::Black && sibling->right->color == QColorConstants::Black)
            {
                sibling->color = QColorConstants::Red;
                node = node->getParentAsSharedPtr();
            }
            else
            {
                if (sibling->right->color == QColorConstants::Black)
                {
                    sibling->color = QColorConstants::Red;
                    sibling->left->color = QColorConstants::Black;
                    this->rightRotate(sibling);
                    sibling = node->getParentAsSharedPtr()->right;
                }

                sibling->color = node->getParentAsSharedPtr()->color;
                node->getParentAsSharedPtr()->color = QColorConstants::Black;
                sibling->right->color = QColorConstants::Black;
                this->leftRotate(node->getParentAsSharedPtr());
                node = this->root;
            }
        }
        else
        {
            sibling = node->getParentAsSharedPtr()->left;
            if (sibling->color == QColorConstants::Red)
            {
                sibling->color = QColorConstants::Black;
                node->getParentAsSharedPtr()->color = QColorConstants::Red;
                this->rightRotate(node->getParentAsSharedPtr());
                sibling = node->getParentAsSharedPtr()->left;
            }

            if (sibling->left->color == QColorConstants::Black && sibling->right->color == QColorConstants::Black)
            {
                sibling->color = QColorConstants::Red;
                node = node->getParentAsSharedPtr();
            }
            else
            {
                if (sibling->left->color == QColorConstants::Black)
                {
                    sibling->color = QColorConstants::Red;
                    sibling->right->color = QColorConstants::Black;
                    this->leftRotate(sibling);
                    sibling = node->getParentAsSharedPtr()->left;
                }

                sibling->color = node->getParentAsSharedPtr()->color;
                node->getParentAsSharedPtr()->color = QColorConstants::Black;
                sibling->left->color = QColorConstants::Black;
                this->rightRotate(node->getParentAsSharedPtr());
                node = this->root;
            }
        }
    }
    node->color = QColorConstants::Black;
}

#endif // REDBLACKTREE_H
