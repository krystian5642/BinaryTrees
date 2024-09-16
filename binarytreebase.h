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

    struct BinaryTreeNode
    {
        BinaryTreeNode(const ValueType &value)
            : value(value)
        {}

        virtual ~BinaryTreeNode() = default;

        virtual shared_ptr<BinaryTreeNode> getParent() const = 0;
        virtual shared_ptr<BinaryTreeNode> getLeft() const = 0;
        virtual shared_ptr<BinaryTreeNode> getRight() const = 0;

        const ValueType& getValue() const { return value; }
        const QColor& getColor() const { return color; }

        ValueType value;
        QColor color = QColorConstants::Black;
    };

    bool add(const ValueType &value);
    bool remove(const ValueType &value);

    const shared_ptr<BinaryTreeNode>& getRoot() const { return root; }
    virtual shared_ptr<BinaryTreeNode> getLeafNode() const { return nullptr; }

protected:   
    virtual shared_ptr<BinaryTreeNode> addInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot, const shared_ptr<BinaryTreeNode> &parent, shared_ptr<BinaryTreeNode> &newNode) = 0;
    virtual shared_ptr<BinaryTreeNode> removeInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot, bool &removed) = 0;
    virtual shared_ptr<BinaryTreeNode> createNode(const ValueType &value) const = 0;
    virtual shared_ptr<BinaryTreeNode> getMaxValuePtr(const shared_ptr<BinaryTreeNode> &inRoot) const = 0;
    virtual shared_ptr<BinaryTreeNode> getMinValuePtr(const shared_ptr<BinaryTreeNode> &inRoot) const = 0;

    virtual void postAddInternal(const shared_ptr<BinaryTreeNode> &newNode) {};
    virtual void postRemoveInternal() {};
    virtual shared_ptr<BinaryTreeNode> getNodeForValue(const ValueType &value) const { return nullptr; }

    template <class NodeClass>
    shared_ptr<NodeClass> getNodeAs(const shared_ptr<BinaryTreeNode> &inRoot) const;

protected:
   shared_ptr<BinaryTreeNode> root;
};

template<class ValueType>
inline bool BinaryTreeBase<ValueType>::add(const ValueType &value)
{
    shared_ptr<BinaryTreeNode> newNode;
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

template<class ValueType> template<class NodeClass>
inline shared_ptr<NodeClass> BinaryTreeBase<ValueType>::getNodeAs(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    return std::dynamic_pointer_cast<NodeClass>(inRoot);
}

#endif // BINARYTREEBASE_H
