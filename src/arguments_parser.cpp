#include "arguments_parser.h"

ArgumentsParser::ArgumentsParser() {
}

ArgumentsParser::ArgumentsParser(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        line_.push_back(argv[i]);
    }
}

void ArgumentsParser::AddArgument(char short_name, const std::string &long_name, const std::string &type,
                                  const std::string &help, std::string *store_to) {
    ArgumentsHolder arguments_holder;
    arguments_holder.ShortName = short_name;
    arguments_holder.LongName = long_name;
    arguments_holder.Type = type;
    arguments_holder.Help = help;
    arguments_holder.ArgType = TArgType::ARGUMENT;
    arguments_holder.store_to_argument = store_to;
    args_[short_name] = arguments_holder;
    help_message_ += "-" + std::string(1, short_name) + " or " + "--" + long_name + " " + type + " -- " + help + "\n";
}

void ArgumentsParser::AddMultipleArguments(const std::string &type, const std::string &help,
                                           std::vector<std::string> *store_to) {
    ArgumentsHolder arguments_holder;
    arguments_holder.Type = type;
    arguments_holder.Help = help;
    arguments_holder.ArgType = TArgType::MULTIPLE_ARGUMENTS;
    arguments_holder.store_to_arguments = store_to;
    args_[0] = arguments_holder;
    help_message_ += type + " -- " + help + "\n";
}

void ArgumentsParser::AddFlag(char short_name, const std::string &long_name, const std::string &help, bool *store_to) {
    ArgumentsHolder arguments_holder;
    arguments_holder.ShortName = short_name;
    arguments_holder.LongName = long_name;
    arguments_holder.Help = help;
    arguments_holder.ArgType = TArgType::FLAG;
    arguments_holder.store_to_flag = store_to;
    args_[short_name] = arguments_holder;
    help_message_ += "-" + std::string(1, short_name) + " or " + "--" + long_name + " -- " + help + "\n";
}

void ArgumentsParser::Parse() {
    for (size_t i = 0; i < line_.size();) {
        if (line_[i][0] == '-') {
            std::string cur = line_[i];
            while (!cur.empty() && cur[0] == '-') {
                cur.erase(cur.begin());
            }
            if (cur.empty()) {
                ++i;
                continue;
            }

            for (auto &argument_holder : args_) {
                if (argument_holder.second.ShortName == cur || argument_holder.second.LongName == cur) {
                    if (argument_holder.second.ArgType == TArgType::FLAG) {
                        *argument_holder.second.store_to_flag = true;
                        break;
                    }
                    if (i + 1 < line_.size() && line_[i + 1][0] != '-') {
                        if (argument_holder.second.ArgType == TArgType::ARGUMENT) {
                            *argument_holder.second.store_to_argument = line_[i + 1];
                            break;
                        }
                    }
                }
            }
            i += 2;
        } else {
            std::vector<std::string> multiple_arguments;
            for (size_t j = i; j < line_.size(); j++) {
                multiple_arguments.push_back(line_[j]);
            }
            if (args_.find(0) != args_.end()) {
                *args_[0].store_to_arguments = multiple_arguments;
            }
            break;
        }
    }
}

std::string ArgumentsParser::GetHelp() const {
    return help_message_;
}
