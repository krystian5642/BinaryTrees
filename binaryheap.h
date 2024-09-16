#ifndef HEAP_H
#define HEAP_H

#include "binarytreebase.h"
#include <vector>

// Min Heap
template <class ValueType>
class BinaryHeap : public BinaryTreeBase<ValueType>
{
public:
    BinaryHeap() = default;

    using BinaryTreeNode = typename BinaryTreeBase<ValueType>::BinaryTreeNode;

    struct BinaryHeapNode : public BinaryTreeNode
    {
        BinaryHeapNode(const ValueType& value)
            : BinaryTreeNode(value)
        {
            // temp !
            priority = value;
        }

        inline int getParentIndex() const { return index != 0 ? (index - 1) / 2 : -1; }
        inline int getLeftIndex() const { return 2 * index + 1; }
        inline int getRightIndex() const { return 2 * index + 2; }

        virtual shared_ptr<BinaryTreeNode> getParent() const override
        {
            const int parentIndex = getParentIndex();
            return parentIndex >= 0 && parentIndex < heap->nodes.size() ? heap->nodes[parentIndex] : nullptr;
        }

        virtual shared_ptr<BinaryTreeNode> getLeft() const override
        {
            const int leftIndex = getLeftIndex();
            return leftIndex < heap->nodes.size() ? heap->nodes[leftIndex] : nullptr;
        }

        virtual shared_ptr<BinaryTreeNode> getRight() const override
        {
            const int rightIndex = getRightIndex();
            return rightIndex < heap->nodes.size() ? heap->nodes[rightIndex] : nullptr;
        }

        int index = 0;
        float priority = 0.f;

        BinaryHeap* heap;
    };

    ValueType extractMin();

    void updateValue(const ValueType& oldValue, const ValueType& newValue);

protected:
    virtual shared_ptr<BinaryTreeNode> addInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot, const shared_ptr<BinaryTreeNode> &removed, shared_ptr<BinaryTreeNode> &newNode) override;
    virtual shared_ptr<BinaryTreeNode> removeInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot, bool &removed) override;
    virtual shared_ptr<BinaryTreeNode> createNode(const ValueType &value) const override;
    virtual shared_ptr<BinaryTreeNode> getMaxValuePtr(const shared_ptr<BinaryTreeNode> &inRoot) const override;
    virtual shared_ptr<BinaryTreeNode> getMinValuePtr(const shared_ptr<BinaryTreeNode> &inRoot) const override;

    virtual void postAddInternal(const shared_ptr<BinaryTreeNode> &newNode) override;

    void shiftUp(const shared_ptr<BinaryHeapNode> &inRoot);
    void shiftDown(const shared_ptr<BinaryHeapNode> &inRoot);

    void swap(const shared_ptr<BinaryHeapNode> &x, const shared_ptr<BinaryHeapNode> &y);

protected:
    std::vector<shared_ptr<BinaryHeapNode>> nodes;
};

template<class ValueType>
ValueType BinaryHeap<ValueType>::extractMin()
{
    if(nodes.empty())
    {
        return ValueType{};
    }

    if(nodes.size() == 1)
    {
        return nodes[0];
    }

    std::swap(nodes[0], nodes[nodes.size() - 1]);

    const auto last = nodes.back();
    nodes.pop_back();

    shiftDown(nodes[0]);

    return last;
}

template<class ValueType>
void BinaryHeap<ValueType>::updateValue(const ValueType &oldValue, const ValueType &newValue)
{
    const auto oldValueIt = std::find_if(nodes.begin(), nodes.end(), [&oldValue](const shared_ptr<BinaryHeapNode>& heapNode)
    {
        return heapNode->value == oldValue;
    });

    if(oldValueIt != nodes.end())
    {
        const auto oldValuePtr = *oldValueIt;
        oldValuePtr->value = newValue;

        const auto parentPtr = oldValuePtr->getParent();
        if(parentPtr && newValue < parentPtr->value)
        {
            shiftUp(oldValuePtr);
        }
        else
        {
            shiftDown(oldValuePtr);
        }
    }
}

template <class ValueType>
shared_ptr<typename BinaryHeap<ValueType>::BinaryTreeNode> BinaryHeap<ValueType>::addInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot, const shared_ptr<BinaryTreeNode> &parent, shared_ptr<BinaryTreeNode> &newNode)
{
    const auto heapNodePtr = this->template getNodeAs<BinaryHeapNode>(this->createNode(value));
    heapNodePtr->index = nodes.size();
    heapNodePtr->heap = this;
    nodes.push_back(heapNodePtr);
    newNode = heapNodePtr;
    return nodes[0];
}

template <class ValueType>
shared_ptr<typename BinaryHeap<ValueType>::BinaryTreeNode> BinaryHeap<ValueType>::removeInternal(const ValueType &value, const shared_ptr<BinaryTreeNode> &inRoot, bool &removed)
{
    return nodes[0];
}

template<class ValueType>
inline shared_ptr<typename BinaryHeap<ValueType>::BinaryTreeNode> BinaryHeap<ValueType>::createNode(const ValueType &value) const
{
    return std::make_shared<BinaryHeapNode>(value);
}

template<class ValueType>
inline shared_ptr<typename BinaryHeap<ValueType>::BinaryTreeNode> BinaryHeap<ValueType>::getMaxValuePtr(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    return nodes[0];
}

template<class ValueType>
inline shared_ptr<typename BinaryHeap<ValueType>::BinaryTreeNode> BinaryHeap<ValueType>::getMinValuePtr(const shared_ptr<BinaryTreeNode> &inRoot) const
{
    return nodes[0];
}

template<class ValueType>
inline void BinaryHeap<ValueType>::postAddInternal(const shared_ptr<BinaryTreeNode> &newNode)
{
    if(newNode)
    {
        shiftUp(this->template getNodeAs<BinaryHeapNode>(newNode));
    }
}

template<class ValueType>
inline void BinaryHeap<ValueType>::shiftUp(const shared_ptr<BinaryHeapNode> &inRoot)
{
    auto parent = this->template getNodeAs<BinaryHeapNode>(inRoot->getParent());
    while(parent && parent->priority > inRoot->priority)
    {
        swap(inRoot, parent);
        parent = this->template getNodeAs<BinaryHeapNode>(inRoot->getParent());
    }

    this->root = nodes[0];
}

template<class ValueType>
inline void BinaryHeap<ValueType>::shiftDown(const shared_ptr<BinaryHeapNode> &inRoot)
{
    auto leftChild = this->template getNodeAs<BinaryHeapNode>(inRoot->getLeft());
    auto rightChild = this->template getNodeAs<BinaryHeapNode>(inRoot->getRight());
    while((leftChild && leftChild->priority < inRoot->priority)
           || (rightChild && rightChild->priority < inRoot->priority))
    {
        if(leftChild && !rightChild)
        {
            swap(inRoot, leftChild);
        }
        else if(!leftChild && rightChild)
        {
            swap(inRoot, rightChild);
        }
        else
        {
            const auto smallest = leftChild->priority > rightChild->priority ? rightChild : leftChild;
            swap(inRoot, smallest);
        }

        leftChild = this->template getNodeAs<BinaryHeapNode>(inRoot->getLeft());
        rightChild = this->template getNodeAs<BinaryHeapNode>(inRoot->getRight());
    }

    this->root = nodes[0];
}

template<class ValueType>
inline void BinaryHeap<ValueType>::swap(const shared_ptr<BinaryHeapNode> &x, const shared_ptr<BinaryHeapNode> &y)
{
    std::swap(nodes[x->index], nodes[y->index]);
    std::swap(x->index, y->index);
}

#endif // HEAP_H
