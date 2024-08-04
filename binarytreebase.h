#ifndef BINARYTREEBASE_H
#define BINARYTREEBASE_H

#include <memory>

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

        T value;

        std::weak_ptr<node> parent;
        std::shared_ptr<node> left;
        std::shared_ptr<node> right;
    };

    bool add(const T& value)
    {
        return addInternal(value, root, nullptr);
    }

    bool remove(const T& value)
    {
        return removeInternal(value, root);
    }

    node* getRoot() const { return root.get(); }

protected:
    virtual bool addInternal(const T& value, std::shared_ptr<node>& in_root, const std::shared_ptr<node>& in_parent) = 0;
    virtual bool removeInternal(const T& value, std::shared_ptr<node>& in_root) = 0;

    std::shared_ptr<node>& getMaxValuePtr(const std::shared_ptr<node>& in_root) const
    {
        return in_root && in_root->right ? getMaxValuePtr(in_root->right) : const_cast<std::shared_ptr<node>&>(in_root);
    }

private:
    std::shared_ptr<node> root;
};

#endif // BINARYTREEBASE_H
