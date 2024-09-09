#ifndef BALANCEDBINARYTREE_H
#define BALANCEDBINARYTREE_H

#include "binarysearchtree.h"

template<class T>
class BalancedBinaryTree : public BinarySearchTree<T>
{
public:
    BalancedBinaryTree() {}

    using Super = BinarySearchTree<T>;
    using node = typename BinaryTreeBase<T>::node;

protected:
    virtual void postAddInternal(const shared_ptr<node>& newNode) override;
    virtual void postRemoveInternal() override;

    void fixRotations(const shared_ptr<node>& inRoot);
};

template<class T>
inline void BalancedBinaryTree<T>::postAddInternal(const shared_ptr<node> &newNode)
{
    fixRotations(this->root);
}

template<class T>
inline void BalancedBinaryTree<T>::postRemoveInternal()
{
    fixRotations(this->root);
}

template<class T>
inline void BalancedBinaryTree<T>::fixRotations(const shared_ptr<node> &inRoot)
{
    if(!inRoot)
    {
        return;
    }

    fixRotations(inRoot->left);

    const int balance_factor = this->getBalanceFactor(inRoot);
    if(balance_factor == 2)
    {
        if(this->getBalanceFactor(inRoot->right) >= 0)
        {
            this->rotateRight(inRoot);
        }
        else
        {
            this->rotateLeft(inRoot->left);
            this->rotateRight(inRoot);
        }
    }
    else if(balance_factor == -2)
    {
        if(this->getBalanceFactor(inRoot->left) <= 0)
        {
            this->rotateLeft(inRoot);
        }
        else
        {
            this->rotateRight(inRoot->right);
            this->rotateLeft(inRoot);
        }
    }

    fixRotations(inRoot->right);
}


#endif // BALANCEDBINARYTREE_H
