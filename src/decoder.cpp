#include "decoder.h"

void Decoder::DecodeArchive() {
    while (!reader_.End()) {
        DecodeFile();
    }
}

void Decoder::DecodeFile() {
    size_t symbols_count = GetVal(reader_.GetWord(Constants::SYMBOL_SIZE));
    std::vector<std::vector<bool>> symbols(symbols_count);
    for (size_t i = 0; i < symbols_count; i++) {
        symbols[i] = reader_.GetWord(Constants::SYMBOL_SIZE);
    }

    std::vector<size_t> count_size;
    count_size.push_back(0);
    size_t sum_count = 0;

    while (sum_count < symbols_count) {
        size_t cur_count = GetVal(reader_.GetWord(Constants::SYMBOL_SIZE));
        if (sum_count + cur_count > symbols_count) {
            throw DecoderException("Wrong archive format: too large sum of symbols counts");
        }
        count_size.push_back(cur_count);
        sum_count += cur_count;
    }

    std::vector<size_t> lengths;

    for (size_t i = 1; i < count_size.size(); i++) {
        while (count_size[i] > 0) {
            --count_size[i];
            lengths.push_back(i);
        }
    }

    auto canonical_codes = GetCanonicalCodes(lengths);

    WriteFile(symbols, canonical_codes);
}

std::vector<bool> Decoder::GetCodeSymbol(Trie& trie) {
    std::vector<bool> code;
    trie.CheckStart();
    bool b = reader_.GetBit();
    while (!trie.CheckAdd(b)) {
        code.push_back(b);
        b = reader_.GetBit();
    }
    code.push_back(b);
    return code;
}

Decoder::Decoder(const std::string& file_name) : reader_(file_name) {
}

void Decoder::WriteFile(std::vector<std::vector<bool>>& symbols, std::vector<std::vector<bool>>& canonical_codes) {
    size_t symbols_count = symbols.size();
    std::unordered_map<std::vector<bool>, std::vector<bool>> get_symbol;

    for (int i = 0; i < symbols_count; i++) {
        get_symbol[canonical_codes[i]] = symbols[i];
    }

    Trie trie(canonical_codes);

    std::string file_name;

    const auto filename_end_vector = GetVector(Constants::FILENAME_END, Constants::SYMBOL_SIZE);
    const auto one_more_file_vector = GetVector(Constants::ONE_MORE_FILE, Constants::SYMBOL_SIZE);
    const auto archive_end_vector = GetVector(Constants::ARCHIVE_END, Constants::SYMBOL_SIZE);

    std::vector<bool> ch;
    try {
        ch = get_symbol[GetCodeSymbol(trie)];
    } catch (const std::out_of_range& e) {
        throw DecoderException("Wrong archive format: no such code");
    }

    while (ch != filename_end_vector) {
        file_name.push_back(static_cast<char>(GetVal(ch)));
        try {
            ch = get_symbol[GetCodeSymbol(trie)];
        } catch (const std::out_of_range& e) {
            throw DecoderException("Wrong archive format: no such code");
        }
    }

    Writer writer(file_name);

    try {
        ch = get_symbol[GetCodeSymbol(trie)];
    } catch (const std::out_of_range& e) {
        throw DecoderException("Wrong archive format: no such code");
    }
    while (ch != one_more_file_vector && ch != archive_end_vector) {
        ch.erase(ch.begin());
        writer.PutWord(ch);
        try {
            ch = get_symbol[GetCodeSymbol(trie)];
        } catch (const std::out_of_range& e) {
            throw DecoderException("Wrong archive format: no such code");
        }
    }
    writer.Flush();
}
