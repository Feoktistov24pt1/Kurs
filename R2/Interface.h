#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <cstdint>

struct Params {
    std::string address;
    uint16_t port = 0;
    std::string input_file;
    std::string output_file;
    std::string credentials_file;
};

class Interface {
public:
    bool parse(int argc, char* argv[], Params& params, std::string& help_text);
};

#endif
