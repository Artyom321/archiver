#pragma once

#include "bin_io.h"
#include "canonical_codes.h"
#include "constants.h"
#include "trie.h"

#include <algorithm>
#include <string>
#include <unordered_map>

class Decoder {
public:
    class DecoderException : public std::exception {
    public:
        DecoderException() {
        }

        explicit DecoderException(std::string msg) : message_(msg) {
        }

        const char* what() const noexcept override {
            return message_.data();
        }

    private:
        const std::string message_;
    };

    explicit Decoder(const std::string& file_name);

    void DecodeArchive();

private:
    Reader reader_;

    void DecodeFile();

    std::vector<bool> GetCodeSymbol(Trie& trie);

    void WriteFile(std::vector<std::vector<bool>>& symbols, std::vector<std::vector<bool>>& canonical_codes);
};
