#include "fastodp.h"
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cryptopp/aes.h>

CryptoPP::SecByteBlock makeKey(std::string keyString) {
    keyString += "odpodpodpodpodpodpodpodpodpodpodpodpodp";
    keyString = keyString.substr(0, 32);
    const char *keyArray = keyString.data();
    CryptoPP::SecByteBlock key((unsigned char *) keyArray, 32);
    return key;
}

std::string FastODP::encrypt(std::string plain, std::string keyString) {
    std::string b64, encrypted;
    CryptoPP::SecByteBlock key = makeKey(keyString);
    CryptoPP::StringSource(plain, true,
                           new CryptoPP::Base64Encoder(new CryptoPP::StringSink(b64), false)); // inner base64
    b64 = "local@" + b64; // add Fast-ODP signature (local@)
    CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption e;
    e.SetKey(key, key.size()); // initialize encrypt instance
    CryptoPP::StringSource(b64, true, new CryptoPP::StreamTransformationFilter(
            e, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(encrypted), false),
            CryptoPP::StreamTransformationFilter::PKCS_PADDING
    )); // AES_256_ECB encrypt
    encrypted = "odp://" + encrypted.substr(0, encrypted.size()) + "/"; // add odp header (odp://.../)
    return encrypted;
}

std::string FastODP::decrypt(
        std::string encrypted,
        std::string keyString) {
    CryptoPP::SecByteBlock key = makeKey(keyString);
    encrypted = encrypted.substr(6, encrypted.size() - 7); // eliminate odp header (odp://.../)
    std::string deB64;
    CryptoPP::StringSource(encrypted, true,
                           new CryptoPP::Base64Decoder(new CryptoPP::StringSink(deB64))); // outer base64
    CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption d(key, key.size()); // initialize decrypt instance
    std::string deAES;
    CryptoPP::StringSource(deB64, true, new CryptoPP::StreamTransformationFilter(
            d, new CryptoPP::StringSink(deAES), CryptoPP::StreamTransformationFilter::PKCS_PADDING
    )); // AES_256_ECB decrypt
    deAES = deAES.substr(6); // eliminate Fast-ODP signature (local@)
    std::string plain;
    CryptoPP::StringSource(deAES, true,
                           new CryptoPP::Base64Decoder(new CryptoPP::StringSink(plain))); // inner base64
    return plain;
}