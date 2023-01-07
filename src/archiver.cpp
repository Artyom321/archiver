#include "command_handler.h"
#include "encoder.h"
#include "decoder.h"

#include <iostream>

int main(int argc, char** argv) {
    CommandHandler reader(argc, argv);
    try {
        auto action = reader.GetAction();
        if (action.first == CommandHandler::Action::HELP) {
            std::cout << action.second["help_message"][0] << std::endl;
            return 0;
        } else if (action.first == CommandHandler::Action::ENCODE) {
            Encoder encoder(action.second["archive_name"][0]);
            encoder.EncodeFiles(action.second["files_to_compress"]);
            std::cout << "Compressed successfully" << std::endl;
        } else {
            Decoder decoder(action.second["archive_name"][0]);
            decoder.DecodeArchive();
            std::cout << "Decompressed successfully" << std::endl;
        }
    } catch (const CommandHandler::CommandHandlerException& e) {
        std::cerr << e.what() << std::endl;
        exit(111);
    } catch (const Reader::ReaderException& e) {
        std::cerr << e.what() << std::endl;
        exit(111);
    } catch (const Decoder::DecoderException& e) {
        std::cerr << e.what() << std::endl;
        exit(111);
    } catch (const Trie::TrieException& e) {
        std::cerr << e.what() << std::endl;
        exit(111);
    } catch (const std::exception& e) {
        std::cerr << "Unknown error";
        exit(111);
    }
    return 0;
}
