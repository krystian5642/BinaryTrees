#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "binarysearchtree.h"

template<class T>
class RedBlackTree : public BinarySearchTree<T>
{
public:
    RedBlackTree() {}

    using Super = BinarySearchTree<T>;
    using node = typename BinaryTreeBase<T>::node;

    struct red_black_node : public Super::node
    {
        red_black_node(const T& value)
            : Super::node(value)
        {
        }

        bool is_red = true;
    };

protected:
    virtual void postAddInternal(const shared_ptr<node>& newNode) override;

    virtual shared_ptr<node> createNode(const T &value) const override
    {
        return std::make_shared<red_black_node>(value);
    }
};

template<class T>
inline void RedBlackTree<T>::postAddInternal(const shared_ptr<node> &newNode)
{
    if(!newNode)
    {
        return;
    }

    auto red_black_node_X = dynamic_pointer_cast<red_black_node>(newNode);
    assert(red_black_node_X);

    if(red_black_node_X == this->root)
    {
        red_black_node_X->is_red = false;
        return;
    }

    auto red_black_parent = dynamic_pointer_cast<red_black_node>(red_black_node_X->parent.lock());
    while(red_black_node_X.get() != this->getRoot() && red_black_parent && red_black_parent->is_red)
    {
        const auto red_black_grandparent = dynamic_pointer_cast<red_black_node>(red_black_parent->parent.lock());
        if(red_black_parent == red_black_grandparent->left)
        {
            const auto red_black_uncle = dynamic_pointer_cast<red_black_node>(red_black_grandparent->right);
            if(red_black_uncle && red_black_uncle->is_red)
            {
                if(red_black_parent->right == red_black_node_X)
                {
                    this->rotateLeft(red_black_parent);

                    red_black_node_X->is_red = false;
                }
                else
                {
                    red_black_parent->is_red = false;
                    red_black_node_X->is_red = true;
                }

                if(red_black_grandparent != this->root)
                {
                    red_black_grandparent->is_red = true;
                }
                red_black_uncle->is_red = false;
            }
            else
            {
                if(red_black_parent->right == red_black_node_X)
                {
                    this->rotateLeft(red_black_parent);
                    this->rotateRight(red_black_grandparent);

                    red_black_node_X->is_red = false;
                }
                else if(red_black_parent->left == red_black_node_X)
                {
                    this->rotateRight(red_black_grandparent);

                    red_black_parent->is_red = false;
                }

                if(red_black_grandparent != this->root)
                {
                    red_black_grandparent->is_red = true;
                }
            }

            red_black_node_X = red_black_grandparent;
        }
        else if(red_black_parent == red_black_grandparent->right)
        {
            const auto red_black_uncle = dynamic_pointer_cast<red_black_node>(red_black_grandparent->left);
            if(red_black_uncle && red_black_uncle->is_red)
            {
                if(red_black_parent->left == red_black_node_X)
                {
                    this->rotateRight(red_black_parent);

                    red_black_node_X->is_red = false;
                }
                else
                {
                    red_black_parent->is_red = false;
                    red_black_node_X->is_red = true;
                }

                if(red_black_grandparent != this->root)
                {
                    red_black_grandparent->is_red = true;
                }

                red_black_uncle->is_red = false;
            }
            else
            {
                if(red_black_parent->left == red_black_node_X)
                {
                    this->rotateRight(red_black_parent);
                    this->rotateLeft(red_black_grandparent);

                    red_black_node_X->is_red = false;
                }
                else if(red_black_parent->right == red_black_node_X)
                {
                    this->rotateLeft(red_black_grandparent);

                    red_black_parent->is_red = false;
                }

                if(red_black_grandparent != this->root)
                {
                    red_black_grandparent->is_red = true;
                }
            }
        }

        red_black_parent = dynamic_pointer_cast<red_black_node>(red_black_node_X->parent.lock());
    }
}


#endif // REDBLACKTREE_H























