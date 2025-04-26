#ifndef CHRONICLE_H
#define CHRONICLE_H
#include <vector>
#include <string>
#include "core/config.hpp"
#include "core/ssh.hpp"

std::vector<std::string> getConfig(connectionInfo ci);
std::vector<std::string> getInterfaces(connectionInfo ci);
#endif // CHRONICLE_H