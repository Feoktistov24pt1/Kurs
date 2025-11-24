#ifndef DATACLIENT_H
#define DATACLIENT_H

#include <string>
#include <cstdint>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>

class DataClient {
public:
    DataClient(const std::string& address, uint16_t port);
    ~DataClient();

    void authenticate(const std::string& login, const std::string& password);
    std::vector<uint32_t> sendVectors(const std::vector<std::vector<uint16_t>>& vectors);

private:
    static ssize_t send_all(int sock, const void* buf, size_t len);

    int sock;
    sockaddr_in server_addr;
};

#endif
