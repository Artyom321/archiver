#pragma once

#include <string_view>
#include <map>
#include <vector>
#include <string>
#include <cstring>

class ArgumentsParser {
public:
    ArgumentsParser();

    ArgumentsParser(int argc, char** argv);

    void AddArgument(char short_name, const std::string& long_name, const std::string& type, const std::string& help,
                     std::string* store_to);

    void AddMultipleArguments(const std::string& type, const std::string& help, std::vector<std::string>* store_to);

    void AddFlag(char short_name, const std::string& long_name, const std::string& help, bool* store_to);

    void Parse();

    std::string GetHelp() const;

private:
    enum class TArgType {
        ARGUMENT,
        MULTIPLE_ARGUMENTS,
        FLAG,
    };

    struct ArgumentsHolder {
        std::string Help;
        std::string ShortName;
        std::string LongName;
        std::string Type;
        std::string* store_to_argument = nullptr;
        std::vector<std::string>* store_to_arguments = nullptr;
        bool* store_to_flag = nullptr;
        TArgType ArgType;

        bool FlagValue;
        std::string ArgumentValue;
        std::vector<std::string> ArgumentsValues;
    };

    std::map<char, ArgumentsHolder> args_;

    std::string help_message_;

    std::vector<std::string> line_;
};
