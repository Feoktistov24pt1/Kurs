#include "DataClient.h"
#include "Authentication.h"
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>

DataClient::DataClient(const std::string& address, uint16_t port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) throw std::runtime_error("Не удалось создать сокет");
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, address.c_str(), &server_addr.sin_addr) <= 0)
        throw std::runtime_error("Неверный IP-адрес");
    
    if (connect(sock, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0)
        throw std::runtime_error("Не удалось подключиться к серверу");
}

DataClient::~DataClient() {
    close(sock);
}

void DataClient::authenticate(const std::string& login, const std::string& password) {
    std::string salt = generateSalt16();
    std::string kjfds = salt + password;
    std::string hash = computeSHA1(kjfds);
    std::string msg = login + salt + hash;

    ssize_t sent = send(sock, msg.c_str(), msg.size(), 0);
    if (sent != static_cast<ssize_t>(msg.size()))
        throw std::runtime_error("Ошибка отправки сообщения аутентификации");
    
    char response[3];
    ssize_t received = recv(sock, response, 2, 0);
    
    if (received != 2 || std::string(response, 2) != "OK")
        throw std::runtime_error("Аутентификация не удалась");
    
    std::cout << "Аутентификация успешна!" << std::endl;
}

std::vector<uint32_t> DataClient::sendVectors(const std::vector<std::vector<uint16_t>>& vectors) {
    uint32_t N = vectors.size();

    if (send(sock, &N, sizeof(N), 0) != sizeof(N))
        throw std::runtime_error("Не удалось отправить количество векторов");

    std::vector<uint32_t> results;
    for (size_t i = 0; i < vectors.size(); ++i) {
        const auto& vec = vectors[i];
        
        uint32_t S = vec.size();
        
        if (send(sock, &S, sizeof(S), 0) != sizeof(S))
            throw std::runtime_error("Не удалось отправить размер вектора");

        if (send(sock, vec.data(), vec.size() * sizeof(uint16_t), 0) != 
            static_cast<ssize_t>(vec.size() * sizeof(uint16_t))) {
            throw std::runtime_error("Не удалось отправить данные вектора");
        }

        uint16_t result;
        ssize_t bytes_received = recv(sock, &result, sizeof(result), MSG_WAITALL);
        
        if (bytes_received != sizeof(result))
            throw std::runtime_error("Не удалось получить результат");
        
        results.push_back(result);
    }
    
    std::cout << "Результаты получены и " << std::endl;
    return results;
}
