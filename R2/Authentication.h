#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <string>

std::string generateSalt16();
std::string computeSHA1(const std::string& data);

#endif
