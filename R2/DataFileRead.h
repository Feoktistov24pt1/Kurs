#ifndef DATAFILEREAD_H
#define DATAFILEREAD_H

#include <vector>
#include <cstdint>
#include <string>

using Vector = std::vector<uint16_t>;
using Vectors = std::vector<Vector>;

std::pair<std::string, std::string> loadCredentials(const std::string& filename);
Vectors readInputFile(const std::string& filename);

#endif
