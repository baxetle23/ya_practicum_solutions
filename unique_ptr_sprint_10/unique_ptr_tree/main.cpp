#include <cassert>
#include <iostream>
#include <memory>

template <typename T>
struct TreeNode;

template <typename T>
using TreeNodePtr = std::unique_ptr<TreeNode<T>>;

template <typename T>
struct TreeNode {
    TreeNode(T val, TreeNodePtr<T>&& left, TreeNodePtr<T>&& right)
        : value(std::move(val))
        , left(std::move(left))
        , right(std::move(right)) {
    }

    T value;
    TreeNodePtr<T> left;
    TreeNodePtr<T> right;
    TreeNode* parent = nullptr;
};


template <typename T>
bool CheckTreeProperty_(const TreeNode<T>* node, const T min = -999, const T max = 999) {
	if (!node) {
		return true;
	}
	return node->value >= min && node->value <= max
			&& CheckTreeProperty_(node->left.get(), min, node->value)
			&& CheckTreeProperty_(node->right.get(), node->value, max);
}


template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) noexcept {
    return CheckTreeProperty_(node);
}

template <typename T>
TreeNode<T>* begin(TreeNode<T>* node) noexcept {
    if (node->left)
        return begin(node->left.get());
    return node;
}


template <class T>
T max(TreeNode<T>* node) {
    if (node->parent == nullptr)
    {
        while (node->right != nullptr)
          node = node->right.get();
           return node->value;
    }
    TreeNode<T>* parent_ = node->parent;
    while(parent_->parent != nullptr)
          parent_ = parent_->parent;
    while (parent_->right != nullptr)
          parent_ = parent_->right.get();
    return parent_->value;
}

template <class T>
TreeNode<T>* next(TreeNode<T>* node) {
    if (node->value == max(node))
        return nullptr;
    if (node->right) {
        return begin(node->right.get());
    } else {
        TreeNode<T>* parent_ = node->parent;
        if(parent_->left.get() == node) 
            return parent_;
        if(parent_->right.get() == node){
            while(parent_->parent != nullptr)
                parent_ = parent_->parent;
            return parent_;
        }
    }
    return nullptr;
}

// ??????? ??????? ????? ???? ? ???????? ????????? ? ?????????
TreeNodePtr<int> N(int val, TreeNodePtr<int>&& left = {}, TreeNodePtr<int>&& right = {}) {
    if (left.get() != nullptr) {
        if (left.get()->parent != nullptr) {
            throw std::invalid_argument("error");
        }
    }
    if (right.get() != nullptr) {
        if (right.get()->parent != nullptr) {
            throw std::invalid_argument("error");
        }
    }
    auto res = std::make_unique<TreeNode<int>>(std::move(val), std::move(left), std::move(right));
    if (res.get()->left) {
        res.get()->left->parent = res.get();
    }
    if (res.get()->right) {
        res.get()->right->parent = res.get();
    }
    return res;
}

int main() {
    using namespace std;
    using T = TreeNode<int>;
    auto root1 = N(6, N(4, N(3), N(5)), N(7));
    assert(CheckTreeProperty(root1.get()));

    T* iter = begin(root1.get());
    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

//    auto root2 = N(6, N(4, N(3), N(5)), N(7, N(8)));
//    assert(!CheckTreeProperty(root2.get()));

    // ??????? DeleteTree ?? ?????. ???? ?????? ????? ?????????? ???????
    // ????????? ???????????? unique_ptr
}