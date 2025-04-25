#ifndef CHRONICLE_CONFIG_H
#define CHRONICLE_CONFIG_H
#include "INIReader.h"
#include "core/error_handler.hpp"
#include <string>
#define CHRONICLE_CONFIG "/etc/chronicle/chronicle.ini"
#define CHRONICLE_CONFIG_DEFUALT_PORT 22
#define CHRONICLE_CONFIG_DEFUALT_USER "root"

struct connectionInfo {

    int getVendorId(std::string vendor_name);
    int getDeviceId(int vendor_id, std::string device_name);

    int vendor;
    int device;
    std::string user;
    std::string password;
    std::string host;
    int port;
};

extern INIReader chronicleConfig;
int readChronicleConfig(std::string config_path = CHRONICLE_CONFIG);
#endif // CHRONICLE_CONFIG_H