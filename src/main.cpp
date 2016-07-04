#include <iostream>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>

using namespace std;

string encrypt(string plain, CryptoPP::SecByteBlock &key) {
    string b64, encrypted;
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

string decrypt(string encrypted, CryptoPP::SecByteBlock &key) {
    encrypted = encrypted.substr(6, encrypted.size() - 7); // eliminate odp header (odp://.../)
    string deB64;
    CryptoPP::StringSource(encrypted, true,
                           new CryptoPP::Base64Decoder(new CryptoPP::StringSink(deB64))); // outer base64
    CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption d(key, key.size()); // initialize decrypt instance
    string deAES;
    CryptoPP::StringSource(deB64, true, new CryptoPP::StreamTransformationFilter(
            d, new CryptoPP::StringSink(deAES), CryptoPP::StreamTransformationFilter::PKCS_PADDING
    )); // AES_256_ECB decrypt
    deAES = deAES.substr(6); // eliminate Fast-ODP signature (local@)
    string plain;
    CryptoPP::StringSource(deAES, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(plain))); // inner base64
    return plain;
}

void print_usage(string name) {
    cout << name << " - Command line Fast-ODP encrypt/decrypt tool\n\n"
    << "Usage: " << name << " -e [plain text] [key]\n"
    << "or:    " << name << " -d [encrypted text] [key]\n" << endl;
}


int main(int argc, const char *argv[]) {

    if (argc < 4 || (strcmp(argv[1], "-e") && strcmp(argv[1], "-d"))) {
        print_usage(string(argv[0]));
        return 1;
    }
    string keyString(argv[argc - 1]), rawText(argv[argc - 2]);
    keyString += "odpodpodpodpodpodpodpodpodpodpodpodpodp";
    keyString = keyString.substr(0, 32);
    const char *keyArray = keyString.data();
    CryptoPP::SecByteBlock key((unsigned char *) keyArray, 32);
    string output;

    if (strcmp(argv[1], "-e") == 0) {
        output = encrypt(rawText, key);
    }
    else {
        string encrypted = rawText;
        try {
            output = decrypt(encrypted, key);
        } catch (CryptoPP::InvalidCiphertext invalidCiphertext) {
            cout << "failed to decrypt" << endl;
            return 2;
        }
    }
    cout << output << endl;
    return 0;
}
