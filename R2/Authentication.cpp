#include "Authentication.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <random>

std::string generateSalt16() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    uint64_t salt = gen();

    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(16) << salt;
    return ss.str();
}

std::string computeSHA1(const std::string& data) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

    std::stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(hash[i]);
    return ss.str();
}
