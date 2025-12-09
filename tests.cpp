#include <UnitTest++.h>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>
#include "Interface.h"
#include "Authentication.h"
#include "DataFileRead.h"

// Тесты для Interface

TEST(ParseHelpShort) {
    Interface iface;
    Params p;
    std::string help;
    const char* argv[] = {"client", "-h"};
    CHECK(!iface.parse(2, const_cast<char**>(argv), p, help));
    CHECK(!help.empty());
}

TEST(ParseHelpLong) {
    Interface iface;
    Params p;
    std::string help;
    const char* argv[] = {"client", "--help"};
    CHECK(!iface.parse(2, const_cast<char**>(argv), p, help));
    CHECK(!help.empty());
}

TEST(ParseValidFull) {
    Interface iface;
    Params p;
    std::string help;
    const char* argv[] = {
        "client",
        "-a", "127.0.0.1",
        "-p", "33333",
        "-i", "input.bin",
        "-o", "output.txt",
        "-c", "auth.txt"
    };
    CHECK(iface.parse(11, const_cast<char**>(argv), p, help));
    CHECK_EQUAL(p.address, "127.0.0.1");
    CHECK_EQUAL(p.port, 33333U);
    CHECK_EQUAL(p.input_file, "input.bin");
    CHECK_EQUAL(p.output_file, "output.txt");
    CHECK_EQUAL(p.credentials_file, "auth.txt");
}

TEST(ParseMissingRequired) {
    Interface iface;
    Params p;
    std::string help;
    const char* argv[] = {"client", "-a", "127.0.0.1", "-p", "33333", "-i", "in", "-o", "out"};
    CHECK(!iface.parse(9, const_cast<char**>(argv), p, help));
}

TEST(ParseInvalidPort) {
    Interface iface;
    Params p;
    std::string help;
    const char* argv[] = {"client", "-a", "127.0.0.1", "-p", "70000", "-i", "in", "-o", "out", "-c", "auth"};
    CHECK(!iface.parse(11, const_cast<char**>(argv), p, help));
}

// Тесты для Authentication

TEST(SaltIs16Chars) {
    std::string salt = generateSalt16();
    CHECK_EQUAL(16U, salt.size());
}

TEST(SaltContainsOnlyHexDigits) {
    std::string salt = generateSalt16();
    for (char c : salt) {
        bool is_hex = (c >= '0' && c <= '9') ||
                      (c >= 'a' && c <= 'f') ||
                      (c >= 'A' && c <= 'F');
        CHECK(is_hex);
    }
}

TEST(ComputeSHA1WithRealData) {
    std::string salt = "A1B2C3D4E5F67890";
    std::string password = "P@ssW0rd";
    std::string input = salt + password;
    std::string expected = "61FE4F7FE91582EC1C907F506E0AE0662C26639E";
    std::string actual = computeSHA1(input);

    CHECK_EQUAL(expected, actual);
}

struct ValidAuthFile {
    const char* filename = "test_auth.txt";
    ValidAuthFile() {
        std::ofstream f(filename);
        f << "user:P@ssW0rd";
    }
    ~ValidAuthFile() {
        std::remove(filename);
    }
};

struct EmptyAuthFile {
    const char* filename = "empty_auth.txt";
    EmptyAuthFile() {
        std::ofstream f(filename);
    }
    ~EmptyAuthFile() {
        std::remove(filename);
    }
};

struct ValidBinaryFile {
    const char* filename = "test_input.bin";
    ValidBinaryFile() {
        std::ofstream f(filename, std::ios::binary);
        uint32_t N = 2;
        f.write(reinterpret_cast<const char*>(&N), sizeof(N));

        uint32_t S1 = 3;
        f.write(reinterpret_cast<const char*>(&S1), sizeof(S1));
        uint16_t v1[] = {10, 20, 30};
        f.write(reinterpret_cast<const char*>(v1), sizeof(v1));

        uint32_t S2 = 1;
        f.write(reinterpret_cast<const char*>(&S2), sizeof(S2));
        uint16_t v2[] = {1000};
        f.write(reinterpret_cast<const char*>(v2), sizeof(v2));
    }
    ~ValidBinaryFile() {
        std::remove(filename);
    }
};

// Тесты для DataFileRead

TEST_FIXTURE(ValidAuthFile, LoadCredentialsSuccess) {
    auto [login, pass] = loadCredentials(filename);
    CHECK_EQUAL("user", login);
    CHECK_EQUAL("P@ssW0rd", pass);
}

TEST_FIXTURE(EmptyAuthFile, LoadCredentialsFailsOnEmpty) {
    CHECK_THROW(loadCredentials(filename), std::runtime_error);
}

TEST(LoadCredentialsFileNotFound) {
    CHECK_THROW(loadCredentials("no_such_file.auth"), std::runtime_error);
}

TEST_FIXTURE(ValidBinaryFile, ReadVectorsSuccess) {
    auto vectors = readInputFile(filename);
    CHECK_EQUAL(2U, vectors.size());
    CHECK_EQUAL(3U, vectors[0].size());
    CHECK_EQUAL(10U, vectors[0][0]);
    CHECK_EQUAL(20U, vectors[0][1]);
    CHECK_EQUAL(30U, vectors[0][2]);
    CHECK_EQUAL(1U, vectors[1].size());
    CHECK_EQUAL(1000U, vectors[1][0]);
}

TEST(ReadBinaryFileNotFound) {
    CHECK_THROW(readInputFile("nonexistent.bin"), std::runtime_error);
}

int main() {
    std::cout << "Запуск тестов" << std::endl;
   
    int result = UnitTest::RunAllTests();
    
    std::cout << "Завершение тестов" << std::endl;
    return result;
}
