#include "Interface.h"
#include <iostream>
#include <string>
#include <cstdlib>

bool Interface::parse(int argc, char* argv[], Params& params, std::string& help_text) {
    help_text = R"(Использование: client [ОПЦИИ]

Опции:
  -h, --help              Показать эту справку и выйти
  -a, --address АДРЕС     IP-адрес сервера (обязательный)
  -p, --port ПОРТ         Номер TCP-порта сервера (обязательный)
  -i, --input ФАЙЛ        Входной двоичный файл с векторами (обязательный)
  -o, --output ФАЙЛ       Выходной текстовый файл для результатов (обязательный)
  -c, --credentials ФАЙЛ  Файл с логином и паролем в формате 'login:password' (обязательный)

Пример:
  ./client -a 127.0.0.1 -p 33333 -i input.bin -o output.txt -c auth.txt
)";

    if (argc == 1) return false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            return false;
        } else if (arg == "-a" || arg == "--address") {
            if (++i >= argc) return false;
            params.address = argv[i];
        } else if (arg == "-p" || arg == "--port") {
            if (++i >= argc) return false;
            int p = std::atoi(argv[i]);
            if (p <= 0 || p > 65535) return false;
            params.port = static_cast<uint16_t>(p);
        } else if (arg == "-i" || arg == "--input") {
            if (++i >= argc) return false;
            params.input_file = argv[i];
        } else if (arg == "-o" || arg == "--output") {
            if (++i >= argc) return false;
            params.output_file = argv[i];
        } else if (arg == "-c" || arg == "--credentials") {
            if (++i >= argc) return false;
            params.credentials_file = argv[i];
        } else {
            return false;
        }
    }

    return !params.address.empty() && params.port != 0 &&
           !params.input_file.empty() && !params.output_file.empty() &&
           !params.credentials_file.empty();
}
