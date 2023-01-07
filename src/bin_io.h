#pragma once

#include "constants.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <memory>

class Reader {
public:
    class ReaderException : public std::exception {
    public:
        ReaderException() {
        }

        explicit ReaderException(std::string msg) : message_(msg) {
        }

        const char* what() const noexcept override {
            return message_.data();
        }

    private:
        const std::string message_;
    };

    explicit Reader(const std::string& file);

    explicit Reader(std::istream& in);

    bool GetBit();

    std::vector<bool> GetWord(size_t length);

    int GetChar();

    bool End() const;

private:
    std::unique_ptr<std::istream> in_;

    std::vector<bool> buf_;
};

class Writer {
public:
    explicit Writer(const std::string& file);

    explicit Writer(std::ostream& out);

    void PutBit(bool b);

    void PutWord(const std::vector<bool>& bits);

    void Flush();

    ~Writer();

private:
    std::unique_ptr<std::ostream> out_;

    std::vector<bool> buf_;
};
