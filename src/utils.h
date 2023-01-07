#pragma once

#include <vector>

void Incr(std::vector<bool> &a);

void Shift(std::vector<bool> &a, size_t length);

std::vector<bool> GetVector(int x, size_t length);

std::vector<bool> GetVector(size_t x, size_t length);

size_t GetVal(const std::vector<bool> v);
