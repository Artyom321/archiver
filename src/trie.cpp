#include "trie.h"

Trie Trie::Merge(Trie &left, Trie &right) {
    Trie root(std::min(left.root_->symbol, right.root_->symbol));
    root.root_->left = left.root_;
    root.root_->right = right.root_;
    return root;
}

bool Trie::operator<(const Trie &other) const {
    if (root_ == nullptr) {
        return true;
    } else if (other.root_ == nullptr) {
        return false;
    }
    return root_->symbol < other.root_->symbol;
}

bool Trie::operator==(const Trie &other) const {
    if (root_ == nullptr) {
        return true;
    } else if (other.root_ == nullptr) {
        return false;
    }
    return root_->symbol == other.root_->symbol;
}

std::vector<std::pair<int, size_t>> Trie::GetCodes() const {
    std::vector<std::pair<int, size_t>> res;
    DfsCodes(root_.get(), res, 0);
    return res;
}

Trie::Trie(std::vector<std::vector<bool>> &codes) {
    root_ = std::make_shared<Node>(Node());

    for (auto &code : codes) {
        Node *cur = root_.get();
        for (auto b : code) {
            if (b == 0) {
                if (cur->left == nullptr) {
                    cur->left = std::make_shared<Node>(Node());
                }
                cur = cur->left.get();
            } else {
                if (cur->right == nullptr) {
                    cur->right = std::make_shared<Node>(Node());
                }
                cur = cur->right.get();
            }
        }
    }
}

void Trie::CheckStart() {
    cur_code_ = root_.get();
}

bool Trie::CheckAdd(bool b) {
    if (b == 0) {
        if (cur_code_->left == nullptr) {
            throw TrieException("Wrong archive format: code doesn't exist");
        }
        cur_code_ = cur_code_->left.get();
    } else {
        if (cur_code_->right == nullptr) {
            throw TrieException("Wrong archive format: code doesn't exist");
        }
        cur_code_ = cur_code_->right.get();
    }
    return cur_code_->IsTerminal();
}

void Trie::DfsCodes(Trie::Node *root, std::vector<std::pair<int, size_t>> &res, size_t len) const {
    if (root == nullptr) {
        return;
    }
    if (root->IsTerminal()) {
        res.push_back(std::make_pair(root->symbol, len));
        return;
    }
    if (root->left != nullptr) {
        DfsCodes(root->left.get(), res, len + 1);
    }
    if (root->right != nullptr) {
        DfsCodes(root->right.get(), res, len + 1);
    }
}
