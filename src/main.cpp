#include <iostream>
#include <cstring>
#include <cryptopp/aes.h>
#include "lib/fastodp.h"
#include <boost/program_options.hpp>
#include <fstream>

namespace po = boost::program_options;

po::options_description visible_opts("Options");
po::options_description file_opt("");
po::options_description opts("");
po::positional_options_description p;
std::string input;
std::string output;
std::string key;


void print_usage(std::string name) {
    std::cout << "Command line Fast-ODP encrypt/decrypt tool\n"
              << "Usage: " << name << " [OPTION]... [FILE]\n"
              << "With no FILE, or when FILE is -, read standard input.\n" << std::endl;
    std::cout << visible_opts << std::endl;
}


int main(int argc, const char *argv[]) {
    visible_opts.add_options()
            ("help,h", "produce help message")
            ("encrypt,e", "to encrypt message")
            ("decrypt,d", "to decrypt message")
            ("key,k", po::value<std::string>(&key), "key to encrypt/decrypt (required)");

    file_opt.add_options()
            ("input-file", "");

    opts.add(visible_opts).add(file_opt);

    p.add("input-file", 1);

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).options(opts).positional(p).run(), vm);
        po::notify(vm);
    } catch (std::exception &e) {
        std::cout << e.what() << "\n" << std::endl;
        print_usage(std::string(argv[0]));
        return 1;
    }

    if (vm.count("help") || !vm.count("key") ||
        (vm.count("decrypt") && vm.count("encrypt"))) {
        print_usage(std::string(argv[0]));
        return 1;
    }


    if (vm.count("input-file") && vm["input-file"].as<std::string>() != "-") {
        std::ifstream fs(vm["input-file"].as<std::string>(), std::ios::in);
        if (fs.fail()) {
            print_usage(std::string(argv[0]));
            return 1;
        }
        input.assign((std::istreambuf_iterator<char>(fs)),
                     std::istreambuf_iterator<char>());

    } else {
        input.assign((std::istreambuf_iterator<char>(std::cin)),
                     std::istreambuf_iterator<char>());
    }

    if (vm.count("encrypt")) {
        output = FastODP::encrypt(input, key);
    } else {
        try {
            output = FastODP::decrypt(input, key);
        } catch (CryptoPP::InvalidCiphertext invalidCiphertext) {
            std::cout << "failed to decrypt" << std::endl;
            return 2;
        }
    }

    std::cout << output;
    return 0;
}
