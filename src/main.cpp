#include <iostream>
#include <cstring>
#include <cryptopp/aes.h>
#include "lib/fastodp.h"

void print_usage(std::string name) {
    std::cout << name << " - Command line Fast-ODP encrypt/decrypt tool\n\n"
    << "Usage: " << name << " -e [plain text] [key]\n"
    << "or:    " << name << " -d [encrypted text] [key]\n" << std::endl;
}

int main(int argc, const char *argv[]) {

    if (argc < 4 || (strcmp(argv[1], "-e") && strcmp(argv[1], "-d"))) {
        print_usage(std::string(argv[0]));
        return 1;
    }
    std::string keyString(argv[argc - 1]), rawText(argv[argc - 2]);
    std::string output;

    if (strcmp(argv[1], "-e") == 0) {
        output = FastODP::encrypt(rawText, keyString);
    }
    else {
        std::string encrypted = rawText;
        try {
            output = FastODP::decrypt(encrypted, keyString);
        } catch (CryptoPP::InvalidCiphertext invalidCiphertext) {
            std::cout << "failed to decrypt" << std::endl;
            return 2;
        }
    }
    std::cout << output << std::endl;
    return 0;
}
