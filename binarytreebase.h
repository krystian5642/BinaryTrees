#ifndef BINARYTREEBASE_H
#define BINARYTREEBASE_H

#include <memory>
#include <QColor.h>

using std::shared_ptr;
using std::weak_ptr;

template <class ValueType>
class BinaryTreeBase
{
public:
    BinaryTreeBase() = default;
    virtual ~BinaryTreeBase() = default;

    struct Node
    {
        Node(const ValueType &value)
            : value(value)
        {}

        shared_ptr<Node> getParentAsSharedPtr() const { return parent.lock(); }

        ValueType value;

        weak_ptr<Node> parent;
        shared_ptr<Node> left;
        shared_ptr<Node> right;

        QColor color = QColorConstants::Red;
    };

    bool add(const ValueType &value);
    bool remove(const ValueType &value);

    const shared_ptr<Node>& getRoot() const { return root; }
    virtual shared_ptr<Node> getLeafNode() const { return nullptr; }

protected:
    virtual shared_ptr<Node> addInternal(const ValueType &value, const shared_ptr<Node> &inRoot, const shared_ptr<Node> &parent, shared_ptr<Node> &newNode) = 0;
    virtual void postAddInternal(const shared_ptr<Node> &newNode) {};
    virtual shared_ptr<Node> removeInternal(const ValueType &value, const shared_ptr<Node> &valuePtr, bool &removed) = 0;
    virtual void postRemoveInternal() {};
    virtual shared_ptr<Node> getNodeForValue(const ValueType &value) const { return nullptr; }
    virtual shared_ptr<Node> createNode(const ValueType &value) const;

    shared_ptr<Node> getMaxValuePtr(const shared_ptr<Node> &inRoot) const;
    shared_ptr<Node> getMinValuePtr(const shared_ptr<Node> &inRoot) const;

protected:
  shared_ptr<Node> root;
};

template<class ValueType>
inline bool BinaryTreeBase<ValueType>::add(const ValueType &value)
{
    shared_ptr<Node> newNode;
    root = addInternal(value, root, nullptr, newNode);
    postAddInternal(newNode);
    return newNode != nullptr;
}

template<class ValueType>
inline bool BinaryTreeBase<ValueType>::remove(const ValueType &value)
{
    bool removed = false;
    root = removeInternal(value, root, removed);
    postRemoveInternal();
    return removed;
}

template<class ValueType>
inline shared_ptr<typename BinaryTreeBase<ValueType>::Node> BinaryTreeBase<ValueType>::createNode(const ValueType &value) const
{
    return std::make_shared<Node>(value);
}

template<class ValueType>
inline shared_ptr<typename BinaryTreeBase<ValueType>::Node> BinaryTreeBase<ValueType>::getMaxValuePtr(const shared_ptr<Node> &inRoot) const
{
    return inRoot != getLeafNode() && inRoot->right != getLeafNode() ? getMaxValuePtr(inRoot->right) : inRoot;
}

template<class ValueType>
inline shared_ptr<typename BinaryTreeBase<ValueType>::Node> BinaryTreeBase<ValueType>::getMinValuePtr(const shared_ptr<Node> &inRoot) const
{
    return inRoot != getLeafNode() && inRoot->left != getLeafNode() ? getMinValuePtr(inRoot->left) : inRoot;
}

#endif // BINARYTREEBASE_H
