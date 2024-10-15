#ifndef BINARYTREEBASE_H
#define BINARYTREEBASE_H

#include <memory>
#include <unordered_map>

#include <QColor.h>
#include <QRandomGenerator>

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

        // For vizualization
        float x, mod, shift = 0;
    };

    bool add(const ValueType &value);
    bool remove(const ValueType &value);
    void randomFill();

    void buildProperties(std::unordered_map<std::string, int>& outProperites) const;

    const shared_ptr<BinaryTreeNode>& getRoot() const { return root; }
    bool isLeafNode(const shared_ptr<BinaryTreeNode> &node) const;
    virtual bool isNodeValid(const shared_ptr<BinaryTreeNode> &node) const;

protected:   
    virtual shared_ptr<BinaryTreeNode> addInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot, const shared_ptr<BinaryTreeNode> &parent, shared_ptr<BinaryTreeNode> &newNode) = 0;
    virtual shared_ptr<BinaryTreeNode> removeInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot, bool &removed) = 0;
    virtual shared_ptr<BinaryTreeNode> createNode(const ValueType &value) const = 0;
    virtual shared_ptr<BinaryTreeNode> getMaxValuePtr(const shared_ptr<BinaryTreeNode> &inRoot) const = 0;
    virtual shared_ptr<BinaryTreeNode> getMinValuePtr(const shared_ptr<BinaryTreeNode> &inRoot) const = 0;

    virtual void postAddInternal(const shared_ptr<BinaryTreeNode> &newNode) {};
    virtual void postRemoveInternal() {};
    virtual shared_ptr<BinaryTreeNode> getNodeForValue(const ValueType &value) const { return nullptr; }

    int getHeight(const shared_ptr<BinaryTreeNode> &inRoot) const;
    ValueType getSumOfLeafNodes(const shared_ptr<BinaryTreeNode> &inRoot) const;
    bool isFull(const shared_ptr<BinaryTreeNode> &inRoot) const;
    bool isDegenerated(const shared_ptr<BinaryTreeNode> &inRoot) const;
    bool isComplete(const shared_ptr<BinaryTreeNode> &inRoot) const;
    int getNodesCount(const shared_ptr<BinaryTreeNode> &inRoot) const;
    int getLeavesCount(const shared_ptr<BinaryTreeNode> &inRoot) const;
    int getInternalNodesCount(const shared_ptr<BinaryTreeNode> &inRoot) const;

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

template<class ValueType>
inline void BinaryTreeBase<ValueType>::randomFill()
{
    const int numberOfNumbers = QRandomGenerator::global()->bounded(1, 11);
    for(int i=0; i < numberOfNumbers; i++)
    {
        const int randomInt = QRandomGenerator::global()->bounded(-10, 101);
        add(randomInt);
    }
}

template<class ValueType>
void BinaryTreeBase<ValueType>::buildProperties(std::unordered_map<std::string, int>& outProperites) const
{
    outProperites["Tree Height"] = this->getHeight(root);

    const auto minValuePtr = this->getMinValuePtr(root);
    outProperites["Min Value"] = isNodeValid(minValuePtr) ? minValuePtr->value : -1;

    const auto maxValuePtr = this->getMaxValuePtr(root);
    outProperites["Max Value"] = isNodeValid(maxValuePtr) ? maxValuePtr->value : -1;

    outProperites["Sum of Leaf Nodes"] = getSumOfLeafNodes(root);

    outProperites["Is Full"] = static_cast<bool>(isFull(root));

    outProperites["Is Degenerated"] = static_cast<bool>(isDegenerated(root));

    outProperites["Nodes Count"] = getNodesCount(root);

    outProperites["Leaves Count"] = getNodesCount(root);
}

template<class ValueType>
inline bool BinaryTreeBase<ValueType>::isLeafNode(const shared_ptr<BinaryTreeNode> &node) const
{
    return isNodeValid(node) && !isNodeValid(node->getLeft()) && !isNodeValid(node->getRight());
}

template<class ValueType>
inline bool BinaryTreeBase<ValueType>::isNodeValid(const shared_ptr<BinaryTreeNode> &node) const
{
    return node != nullptr;
}

template<class ValueType>
inline int BinaryTreeBase<ValueType>::getHeight(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    return this->isNodeValid(inRoot) ? 1 + std::max(getHeight(inRoot->getLeft()), getHeight(inRoot->getRight())) : -1;
}

template<class ValueType>
inline ValueType BinaryTreeBase<ValueType>::getSumOfLeafNodes(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    if(isNodeValid(inRoot))
    {
        return isLeafNode(inRoot) ? inRoot->value : getSumOfLeafNodes(inRoot->getLeft()) + getSumOfLeafNodes(inRoot->getRight());
    }
    return 0;
}

template<class ValueType>
inline bool BinaryTreeBase<ValueType>::isFull(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    if(isNodeValid(inRoot))
    {
        const bool isLeftChildValid = isNodeValid(inRoot->getLeft());
        const bool isRightChildValid = isNodeValid(inRoot->getRight());
        if((isLeftChildValid && isRightChildValid) || (!isLeftChildValid && !isRightChildValid))
        {
            return true;
        }
        return isFull(inRoot->getLeft()) && isFull(inRoot->getRight());
    }
    return false;
}

template<class ValueType>
inline bool BinaryTreeBase<ValueType>::isDegenerated(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    if(isNodeValid(inRoot))
    {
        const bool isLeftChildValid = isNodeValid(inRoot->getLeft());
        const bool isRightChildValid = isNodeValid(inRoot->getRight());
        if((isLeftChildValid && !isRightChildValid) || (!isLeftChildValid && isRightChildValid))
        {
            return true;
        }
        return isDegenerated(inRoot->getLeft()) && isDegenerated(inRoot->getRight());
    }
    return false;
}

template<class ValueType>
inline bool BinaryTreeBase<ValueType>::isComplete(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    return false;
}

template<class ValueType>
inline int BinaryTreeBase<ValueType>::getNodesCount(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    return isNodeValid(inRoot) ? 1 + getNodesCount(inRoot->getLeft()) + getNodesCount(inRoot->getRight()) : 0;
}

template<class ValueType>
inline int BinaryTreeBase<ValueType>::getLeavesCount(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    return isLeafNode(inRoot) ? 1 : getLeavesCount(inRoot->getLeft()) + getLeavesCount(inRoot->getRight());
}

template<class ValueType>
inline int BinaryTreeBase<ValueType>::getInternalNodesCount(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    return getNodesCount(inRoot) - getLeavesCount(inRoot);
}

template<class ValueType> template<class NodeClass>
inline shared_ptr<NodeClass> BinaryTreeBase<ValueType>::getNodeAs(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    return std::dynamic_pointer_cast<NodeClass>(inRoot);
}

#endif // BINARYTREEBASE_H




