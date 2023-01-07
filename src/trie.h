#pragma once

#include <algorithm>
#include <vector>
#include <iostream>
#include <memory>

class Trie {
public:
    class TrieException : public std::exception {
    public:
        TrieException() {
        }

        explicit TrieException(std::string msg) : message_(msg) {
        }

        const char *what() const noexcept override {
            return message_.data();
        }

    private:
        const std::string message_;
    };

    Trie() {
    }

    explicit Trie(int symbol) : root_(new Node(symbol)) {
    }

    explicit Trie(std::vector<std::vector<bool>> &codes);

    static Trie Merge(Trie &l, Trie &r);

    bool operator<(const Trie &other) const;

    bool operator==(const Trie &other) const;

    std::vector<std::pair<int, size_t>> GetCodes() const;

    void CheckStart();

    bool CheckAdd(bool b);

private:
    struct Node {
        int symbol;
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;

        Node() {
        }

        explicit Node(int ch) : symbol(ch) {
        }

        bool IsTerminal() const {
            return left == nullptr && right == nullptr;
        }
    };

    std::shared_ptr<Node> root_ = nullptr;
    Node *cur_code_ = nullptr;

    void DfsCodes(Node *root, std::vector<std::pair<int, size_t>> &res, size_t len) const;
};