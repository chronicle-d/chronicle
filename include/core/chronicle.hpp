#ifndef CHRONICLE_H
#define CHRONICLE_H
#include <vector>
#include <string>
#include "core/config.hpp"
#include "core/ssh.hpp"
#include "core/device_factory.hpp"

std::vector<std::string> getConfig(connectionInfo ci, std::vector<OperationMap> getConfig);
std::vector<std::string> getInterfaces(connectionInfo ci, std::vector<OperationMap> getInterfaces);
#endif // CHRONICLE_H