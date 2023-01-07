#pragma once

#include "arguments_parser.h"

#include <exception>
#include <filesystem>
#include <map>
#include <string>

class CommandHandler {
public:
    class CommandHandlerException : public std::exception {
    public:
        CommandHandlerException() {
        }

        explicit CommandHandlerException(std::string msg) : message_(msg) {
        }

        const char* what() const noexcept override {
            return message_.data();
        }

    private:
        const std::string message_;
    };

    CommandHandler(int argc, char** argv);

    enum class Action {
        ENCODE,
        DECODE,
        HELP,
    };

    std::pair<Action, std::map<std::string, std::vector<std::string>>> GetAction();

private:
    int argc_;
    ArgumentsParser parser_;
};