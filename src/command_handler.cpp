#include "command_handler.h"

#include <iostream>

CommandHandler::CommandHandler(int argc, char** argv) : argc_(argc - 1), parser_(argc, argv) {
}

std::pair<CommandHandler::Action, std::map<std::string, std::vector<std::string>>> CommandHandler::GetAction() {
    std::string compress_archive_name;
    std::string decompress_archive_name;
    std::vector<std::string> files_to_compress;
    bool need_help = false;

    parser_.AddArgument('c', "compress", "archive_name", "add name of compressed archive", &compress_archive_name);
    parser_.AddArgument('d', "decompress", "archive_name", "add name of archive to decompress",
                        &decompress_archive_name);
    parser_.AddFlag('h', "help", "get help info", &need_help);
    parser_.AddMultipleArguments("file1 file2...", "names of files to compress", &files_to_compress);
    parser_.Parse();

    if (!compress_archive_name.empty()) {
        if (argc_ != 2 + files_to_compress.size()) {
            throw CommandHandlerException("Incorrect command\n" + parser_.GetHelp());
        }
        for (const auto& file : files_to_compress) {
            if (!std::filesystem::exists(file)) {
                throw CommandHandlerException("Incorrect command: file " + file + " doesn't exist");
            }
        }
        std::pair<CommandHandler::Action, std::map<std::string, std::vector<std::string>>> res;
        res.first = Action::ENCODE;
        res.second["archive_name"] = std::vector<std::string>(1, compress_archive_name);
        res.second["files_to_compress"] = files_to_compress;
        return res;
    } else if (!decompress_archive_name.empty()) {
        if (argc_ != 2) {
            throw CommandHandlerException("Incorrect command\n" + parser_.GetHelp());
        }
        if (!std::filesystem::exists(decompress_archive_name)) {
            throw CommandHandlerException("Incorrect command: file " + decompress_archive_name + " doesn't exist");
        }
        std::pair<CommandHandler::Action, std::map<std::string, std::vector<std::string>>> res;
        res.first = Action::DECODE;
        res.second["archive_name"] = std::vector<std::string>(1, decompress_archive_name);
        return res;
    } else {
        if (!need_help) {
            throw CommandHandlerException("Incorrect command\n" + parser_.GetHelp());
        } else if (argc_ != 1) {
            throw CommandHandlerException("Incorrect command\n" + parser_.GetHelp());
        }
        std::pair<CommandHandler::Action, std::map<std::string, std::vector<std::string>>> res;
        res.first = Action::HELP;
        res.second["help_message"] = std::vector<std::string>(1, parser_.GetHelp());
        return res;
    }
}
