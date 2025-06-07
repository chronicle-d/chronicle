#ifndef CHRONICLE_CONFIG_H
#define CHRONICLE_CONFIG_H
#include "core/error_handler.hpp"
#include "core/device_factory.hpp"
#include <string>
#define CHRONICLE_CONFIG_DEFUALT_KEX_METHODS "curve25519-sha256@libssh.org,ecdh-sha2-nistp384,ecdh-sha2-nistp256,diffie-hellman-group14-sha256"
#define CHRONICLE_CONFIG_DEFUALT_HOSTKEYS "ssh-ed25519,ecdsa-sha2-nistp521,ecdsa-sha2-nistp384,ecdsa-sha2-nistp256,rsa-sha2-512,rsa-sha2-256"
#define CHRONICLE_CONFIG_DEFUALT_PORT 22
#define CHRONICLE_CONFIG_DEFUALT_USER "chronicle-runner"
#define CHRONICLE_CONFIG_DEFUALT_VERBOSITY 0
#define CHRONICLE_CONFIG_DEFAULT_SSH_IDLE_TIMEOUT 1000
#define CHRONICLE_CONFIG_DEFAULT_SSH_TOTAL_TIMEOUT 10000

struct connectionInfo {

    int getVendorId(std::string vendor_name);
    int getDeviceId(int vendor_id, std::string device_name);

    std::string kex_methods = CHRONICLE_CONFIG_DEFUALT_KEX_METHODS;
    std::string hostkey_algorithms = CHRONICLE_CONFIG_DEFUALT_HOSTKEYS;
    std::string vendorName;
    int vendor;
    std::string deviceName;
    int device;
    std::string user = CHRONICLE_CONFIG_DEFUALT_USER;
    std::string password;
    std::string host;
    int port = CHRONICLE_CONFIG_DEFUALT_PORT;
    int verbosity = CHRONICLE_CONFIG_DEFUALT_VERBOSITY;
};

struct chronicleSettings {
    int ssh_idle_timeout = CHRONICLE_CONFIG_DEFAULT_SSH_IDLE_TIMEOUT;
    int ssh_total_timeout = CHRONICLE_CONFIG_DEFAULT_SSH_TOTAL_TIMEOUT;
};


connectionInfo getConnectionInfo(std::string deviceNickname);
chronicleSettings getChronicleSettings();
#endif // CHRONICLE_CONFIG_H
