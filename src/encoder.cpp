#include "encoder.h"
#include "priority_queue.h"
#include "trie.h"

void Encoder::EncodeFiles(const std::vector<std::string> &files) {
    for (size_t i = 0; i < files.size(); i++) {
        if (i + 1 < files.size()) {
            EncodeFile(files[i], false);
        } else {
            EncodeFile(files[i], true);
        }
    }
}

Encoder::Encoder(const std::string &file) : writer_(file) {
}

void Encoder::EncodeFile(const std::string &file, bool last) {
    std::string file_name = GetName(file);

    Reader reader(file);

    auto chars_count = CountChars(reader, file_name);

    auto codes = GetCodes(chars_count);

    std::vector<size_t> lengths(codes.size());

    for (size_t i = 0; i < codes.size(); i++) {
        lengths[i] = codes[i].second;
    }

    auto canonical_codes = GetCanonicalCodes(lengths);

    WriteFile(file, file_name, codes, canonical_codes, last);
}

std::string Encoder::GetName(const std::string &file_path) const {
    size_t slash_index = file_path.rfind('/');
    return file_path.substr((slash_index == std::string::npos ? 0 : slash_index + 1));
}

std::unordered_map<int, size_t> Encoder::CountChars(Reader &reader, const std::string &file_name) const {
    std::unordered_map<int, size_t> chars_count;
    for (auto &ch : file_name) {
        ++chars_count[ch];
    }

    ++chars_count[Constants::FILENAME_END];
    ++chars_count[Constants::ONE_MORE_FILE];
    ++chars_count[Constants::ARCHIVE_END];

    while (!reader.End()) {
        auto tmp = reader.GetChar();
        ++chars_count[tmp];
    }
    return chars_count;
}

Trie Encoder::BuildTrie(const std::unordered_map<int, size_t> &chars_count) const {
    auto get_min = [](const std::pair<size_t, Trie> &a, const std::pair<size_t, Trie> &b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    };

    PriorityQueue<std::pair<size_t, Trie>, decltype(get_min)> q(get_min);
    for (auto &symbol : chars_count) {
        q.Push(std::make_pair(symbol.second, Trie(symbol.first)));
    }

    while (q.GetSize() != 1) {
        auto l = q.GetTop();
        q.Pop();
        auto r = q.GetTop();
        q.Pop();
        q.Push(std::make_pair(l.first + r.first, Trie::Merge(l.second, r.second)));
    }
    return q.GetTop().second;
}

std::vector<std::pair<int, size_t>> Encoder::GetCodes(const std::unordered_map<int, size_t> &chars_count) const {
    auto codes = BuildTrie(chars_count).GetCodes();

    auto cmp = [](const std::pair<int, size_t> &a, const std::pair<int, size_t> &b) {
        if (a.second != b.second) {
            return a.second < b.second;
        }
        return a.first < b.first;
    };

    std::sort(codes.begin(), codes.end(), cmp);

    return codes;
}

void Encoder::WriteFile(const std::string &file, const std::string &file_name,
                        const std::vector<std::pair<int, size_t>> &codes,
                        const std::vector<std::vector<bool>> &canonical_codes, bool last) {
    writer_.PutWord(GetVector(canonical_codes.size(), Constants::SYMBOL_SIZE));

    for (size_t i = 0; i < canonical_codes.size(); i++) {
        writer_.PutWord(GetVector(codes[i].first, Constants::SYMBOL_SIZE));
    }

    const size_t max_symbol_size = canonical_codes.back().size();

    std::vector<size_t> count_size(max_symbol_size + 1);

    for (auto &canonical_code : canonical_codes) {
        ++count_size[canonical_code.size()];
    }

    for (size_t i = 1; i <= max_symbol_size; i++) {
        writer_.PutWord(GetVector(count_size[i], Constants::SYMBOL_SIZE));
    }

    std::unordered_map<int, size_t> get_code;

    for (size_t i = 0; i < codes.size(); i++) {
        get_code[codes[i].first] = i;
    }

    for (auto &ch : file_name) {
        auto now = canonical_codes[get_code[ch]];
        writer_.PutWord(now);
    }

    writer_.PutWord(canonical_codes[get_code[Constants::FILENAME_END]]);

    Reader reader(file);

    while (!reader.End()) {
        auto ch = reader.GetChar();
        writer_.PutWord(canonical_codes[get_code[ch]]);
    }

    if (!last) {
        writer_.PutWord(canonical_codes[get_code[Constants::ONE_MORE_FILE]]);
    } else {
        writer_.PutWord(canonical_codes[get_code[Constants::ARCHIVE_END]]);
    }
}
