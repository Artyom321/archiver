#include "bin_io.h"

Reader::Reader(const std::string& file) : in_(new std::ifstream(file, std::ios::binary)) {
}

Reader::Reader(std::istream& in) : in_(new std::istream(in.rdbuf())) {
}

bool Reader::GetBit() {
    if (buf_.empty()) {
        if ((*in_).peek() == EOF) {
            throw ReaderException("Unexpected end of file");
        }
        auto ch = (*in_).get();
        for (ssize_t i = 0; i < Constants::CHAR_SIZE; i++) {
            buf_.push_back(ch & (1 << i));
        }
    }
    auto res = buf_.back();
    buf_.pop_back();
    return res;
}

bool Reader::End() const {
    return (*in_).peek() == EOF;
}

std::vector<bool> Reader::GetWord(size_t length) {
    std::vector<bool> res;
    while (res.size() < length) {
        res.push_back(GetBit());
    }
    return res;
}

int Reader::GetChar() {
    if (buf_.empty()) {
        return in_->get();
    } else {
        int x = 0;
        for (ssize_t i = 0; i < Constants::CHAR_SIZE; i++) {
            x *= 2;
            x += GetBit();
        }
        return x;
    }
}

Writer::Writer(const std::string& file) : out_(new std::ofstream(file, std::ios::binary)) {
}

Writer::Writer(std::ostream& out) : out_(new std::ostream(out.rdbuf())) {
}

void Writer::PutBit(bool b) {
    buf_.push_back(b);
    if (buf_.size() == Constants::CHAR_SIZE) {
        int x = 0;
        for (ssize_t i = Constants::CHAR_SIZE - 1; i >= 0; i--) {
            x *= 2;
            x += static_cast<int>(buf_[Constants::CHAR_SIZE - 1 - i]);
        }
        (*out_).put(static_cast<char>(x));
        buf_.clear();
    }
}

void Writer::PutWord(const std::vector<bool>& bits) {
    for (auto b : bits) {
        PutBit(b);
    }
}

void Writer::Flush() {
    while (!buf_.empty()) {
        PutBit(false);
    }
}

Writer::~Writer() {
    Flush();
}
