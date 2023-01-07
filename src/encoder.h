#pragma once

#include "bin_io.h"
#include "constants.h"
#include "priority_queue.h"
#include "trie.h"
#include "canonical_codes.h"

#include <algorithm>
#include <string>
#include <unordered_map>

class Encoder {
public:
    explicit Encoder(const std::string& file);

    void EncodeFiles(const std::vector<std::string>& files);

private:
    Writer writer_;

    void EncodeFile(const std::string& file_path, bool last);

    std::string GetName(const std::string& file_path) const;

    Trie BuildTrie(const std::unordered_map<int, size_t>& chars_count) const;

    std::unordered_map<int, size_t> CountChars(Reader& reader, const std::string& file_name) const;

    std::vector<std::pair<int, size_t>> GetCodes(const std::unordered_map<int, size_t>& chars_count) const;

    void WriteFile(const std::string& file, const std::string& file_name,
                   const std::vector<std::pair<int, size_t>>& codes,
                   const std::vector<std::vector<bool>>& canonical_codes, bool last);
};
