#include "utils.h"

std::vector<bool> GetVector(int x, size_t length) {
    std::vector<bool> res(length);
    for (size_t i = 0; i < length && x > 0; i++) {
        res[i] = (x & (1 << i));
        if (x & (1 << i)) {
            x ^= (1 << i);
        }
    }
    std::reverse(res.begin(), res.end());
    return res;
}

std::vector<bool> GetVector(size_t x, size_t length) {
    std::vector<bool> res(length);
    for (size_t i = 0; i < length && x > 0; i++) {
        res[i] = (x & (1 << i));
        if (x & (1 << i)) {
            x ^= (1 << i);
        }
    }
    std::reverse(res.begin(), res.end());
    return res;
}

void Incr(std::vector<bool>& a) {
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i]) {
            a[i] = false;
        } else {
            a[i] = true;
            return;
        }
    }
    a.push_back(true);
}

void Shift(std::vector<bool>& a, size_t length) {
    auto tmp = a;
    a.clear();
    while (a.size() < length) {
        a.push_back(false);
    }
    for (const auto& b : tmp) {
        a.push_back(b);
    }
}

size_t GetVal(const std::vector<bool> v) {
    size_t x = 0;
    for (size_t i = 0; i < v.size(); i++) {
        x |= (static_cast<size_t>(v[i]) << (v.size() - i - 1));
    }
    return x;
}
