#pragma once

#include <functional>
#include <vector>
#include <algorithm>
#include <stdexcept>

template <typename T, class F = std::function<T(const T&, const T&)>>
class PriorityQueue {
public:
    explicit PriorityQueue(const F& f);

    T GetTop();

    size_t GetSize() const;

    void Push(const T val);

    void Pop();

private:
    std::vector<T> heap_;

    F select_min_;

    void GoUp(size_t ind);

    void GoDown();
};

template <typename T, class F>
PriorityQueue<T, F>::PriorityQueue(const F& f) : select_min_(f) {
}

template <typename T, class F>
T PriorityQueue<T, F>::GetTop() {
    return heap_[0];
}

template <typename T, class F>
size_t PriorityQueue<T, F>::GetSize() const {
    return heap_.size();
}

template <typename T, class F>
void PriorityQueue<T, F>::Push(const T val) {
    heap_.push_back(val);
    GoUp(heap_.size() - 1);
}

template <typename T, class F>
void PriorityQueue<T, F>::Pop() {
    if (heap_.empty()) {
        return;
    }
    heap_[0] = heap_.back();
    heap_.pop_back();
    GoDown();
}

template <typename T, class F>
void PriorityQueue<T, F>::GoUp(size_t ind) {
    while (ind > 0 && select_min_(heap_[ind], heap_[(ind - 1) / 2]) == heap_[ind]) {
        std::swap(heap_[ind], heap_[(ind - 1) / 2]);
        ind = (ind - 1) / 2;
    }
}

template <typename T, class F>
void PriorityQueue<T, F>::GoDown() {
    size_t ind = 0;
    while (ind < heap_.size()) {
        if (ind * 2 + 1 >= heap_.size()) {
            break;
        } else if (ind * 2 + 2 >= heap_.size() && select_min_(heap_[ind], heap_[ind * 2 + 1]) == heap_[ind * 2 + 1]) {
            std::swap(heap_[ind * 2 + 1], heap_[ind]);
            ind = ind * 2 + 1;
        } else {
            if (select_min_(heap_[ind * 2 + 1], heap_[ind]) == heap_[ind] &&
                select_min_(heap_[ind * 2 + 2], heap_[ind]) == heap_[ind]) {
                break;
            }
            if (select_min_(heap_[ind * 2 + 1], heap_[ind * 2 + 2]) == heap_[ind * 2 + 1]) {
                std::swap(heap_[ind * 2 + 1], heap_[ind]);
                ind = ind * 2 + 1;
            } else {
                std::swap(heap_[ind * 2 + 2], heap_[ind]);
                ind = ind * 2 + 2;
            }
        }
    }
}
