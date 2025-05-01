#ifndef CHRONICLE_CONFIG_H
#define CHRONICLE_CONFIG_H
#include "INIReader.h"
#include "core/error_handler.hpp"
#include <string>
#define CHRONICLE_CONFIG "/etc/chronicle/chronicle.ini"
#define CHRONICLE_CONFIG_DEFUALT_KEX_METHODS "curve25519-sha256@libssh.org,ecdh-sha2-nistp384,ecdh-sha2-nistp256,diffie-hellman-group14-sha256"
#define CHRONICLE_CONFIG_DEFUALT_HOSTKEYS "ssh-ed25519,ecdsa-sha2-nistp521,ecdsa-sha2-nistp384,ecdsa-sha2-nistp256,rsa-sha2-512,rsa-sha2-256"
#define CHRONICLE_CONFIG_DEFUALT_PORT 22
#define CHRONICLE_CONFIG_DEFUALT_USER "chronicle-runner"
#define CHRONICLE_CONFIG_DEFUALT_VERBOSITY 0
#define CHRONICLE_SSH_IDLE_TIMEOUT 10000
#define CHRONICLE_SSH_TOTAL_TIMEOUT 500

struct connectionInfo {

    int getVendorId(std::string vendor_name);
    int getDeviceId(int vendor_id, std::string device_name);

    std::string kex_methods = CHRONICLE_CONFIG_DEFUALT_KEX_METHODS;
    std::string hostkey_algorithms = CHRONICLE_CONFIG_DEFUALT_HOSTKEYS;
    int vendor;
    int device;
    std::string user = CHRONICLE_CONFIG_DEFUALT_USER;
    std::string password;
    std::string host;
    int port = CHRONICLE_CONFIG_DEFUALT_PORT;
    int verbosity = CHRONICLE_CONFIG_DEFUALT_VERBOSITY;
};

extern INIReader chronicleConfig;
int readChronicleConfig(std::string config_path = CHRONICLE_CONFIG);
connectionInfo getConnectionInfo(std::string section);
#endif // CHRONICLE_CONFIG_H