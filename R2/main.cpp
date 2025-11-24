#include "Interface.h"
#include "DataFileRead.h"
#include "DataClient.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    try {
        Interface iface;
        Params params;
        std::string help;

        if (!iface.parse(argc, argv, params, help)) {
            std::cout << help;
            return 0;
        }

        auto [login, password] = loadCredentials(params.credentials_file);
        auto vectors = readInputFile(params.input_file);

        DataClient client(params.address, params.port);
        client.authenticate(login, password);
        auto results = client.sendVectors(vectors);

        std::ofstream out(params.output_file);
        if (!out) throw std::runtime_error("Не удалось создать файл результатов");

        out << results.size();
        for (uint32_t r : results) out << " " << r;
        out << "\n";

        std::cout << "сохранены в файл " << params.output_file << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
