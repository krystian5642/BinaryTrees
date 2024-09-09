#ifndef BINARYTREEBASE_H
#define BINARYTREEBASE_H

#include <memory>

using std::weak_ptr;
using std::shared_ptr;
using std::dynamic_pointer_cast;

template<class T>
class BinaryTreeBase
{
public:
    BinaryTreeBase() {}
    virtual ~BinaryTreeBase() {}

    struct node
    {
        node(const T& in_value)
            : value(in_value)
        {
        }

        virtual ~node() = default;

        T value;

        weak_ptr<node> parent;
        shared_ptr<node> left;
        shared_ptr<node> right;
    };

    bool add(const T& value)
    {
        shared_ptr<node> new_node;
        root = addInternal(value, root, nullptr, new_node);
        postAddInternal(new_node);
        return new_node != nullptr;
    }

    bool remove(const T& value)
    {
        bool removed = false;
        root = removeInternal(value, root, removed);
        postRemoveInternal();
        return removed;
    }

    const node* getRoot() const
    {
        return root.get();
    }

protected:
    virtual shared_ptr<node> addInternal(const T &value, const shared_ptr<node>& inRoot, const shared_ptr<node>& parent, shared_ptr<node>& new_node) = 0;
    virtual void postAddInternal(const shared_ptr<node>& newNode) {};
    virtual shared_ptr<node> removeInternal(const T &value, const shared_ptr<node>& valuePtr, bool& removed) = 0;
    virtual void postRemoveInternal() {};
    virtual shared_ptr<node> getNodeForValue(const T &value) const { return nullptr; };

    virtual shared_ptr<node> createNode(const T &value) const
    {
        return std::make_shared<node>(value);
    }

    shared_ptr<node>& getMaxValuePtr(const shared_ptr<node>& inRoot) const
    {
        return inRoot && inRoot->right ? getMaxValuePtr(inRoot->right) : const_cast<shared_ptr<node>&>(inRoot);
    }

    shared_ptr<node>& getMinValuePtr(const shared_ptr<node>& inRoot) const
    {
        return inRoot && inRoot->left ? getMinValuePtr(inRoot->left) : const_cast<shared_ptr<node>&>(inRoot);
    }

protected:
    shared_ptr<node> root;
};

#endif // BINARYTREEBASE_H
