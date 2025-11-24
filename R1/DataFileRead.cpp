#include "DataFileRead.h"
#include <fstream>
#include <stdexcept>

std::pair<std::string, std::string> loadCredentials(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Не удалось открыть файл с логином/паролем");

    std::string line;
    if (!std::getline(file, line)) throw std::runtime_error("Файл с логином/паролем пуст");

    size_t pos = line.find(':');
    if (pos == std::string::npos)
        throw std::runtime_error("Неверный формат файла с логином/паролем");

    return {line.substr(0, pos), line.substr(pos + 1)};
}

Vectors readInputFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Не удалось открыть входной файл");

    uint32_t N;
    file.read(reinterpret_cast<char*>(&N), sizeof(N));
    if (!file || N == 0) throw std::runtime_error("Неверный формат входного файла");

    Vectors result;
    for (uint32_t i = 0; i < N; ++i) {
        uint32_t S;
        file.read(reinterpret_cast<char*>(&S), sizeof(S));
        if (!file || S == 0) throw std::runtime_error("Неверный размер вектора в файле");

        Vector vec(S);
        file.read(reinterpret_cast<char*>(vec.data()), S * sizeof(uint16_t));
        if (!file || file.gcount() != static_cast<std::streamsize>(S * sizeof(uint16_t)))
            throw std::runtime_error("Ошибка чтения данных вектора");

        result.push_back(vec);
    }
    return result;
}
