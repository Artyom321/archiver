#include "canonical_codes.h"

std::vector<std::vector<bool>> GetCanonicalCodes(std::vector<size_t>& lengths) {
    std::vector<std::vector<bool>> canonical_codes;
    std::vector<bool> code(1);
    for (size_t i = 0; i < lengths.size(); i++) {
        std::vector<bool> cur_code = code;
        while (cur_code.size() < lengths[i]) {
            cur_code.push_back(false);
        }
        canonical_codes.push_back(cur_code);
        std::reverse(canonical_codes.back().begin(), canonical_codes.back().end());
        if (i + 1 < lengths.size()) {
            Incr(code);
            Shift(code, lengths[i + 1] - lengths[i]);
        }
    }
    return canonical_codes;
}