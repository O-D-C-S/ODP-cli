#ifndef FASTDRIVER_H
#define FASTDRIVER_H

#include <string>
#include <cryptopp/aes.h>
namespace fastdriver{
    std::string encrypt(std::string, std::string);
    std::string decrypt(std::string, std::string);
}

#endif //FASTDRIVER_H
