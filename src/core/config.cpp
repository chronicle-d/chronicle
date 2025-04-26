#include "core/config.hpp"

INIReader chronicleConfig("");
int readChronicleConfig(std::string config_path) {
    chronicleConfig = INIReader(config_path);

    if (chronicleConfig.ParseError() < 0) {
        throwChronicleException(107, "readChronicleConfig");
    } else {
        return 0;
    }

    return 0;
}

int connectionInfo::getVendorId(std::string vendor_name) {
    if (vendor_name == "Cisco") {
        return 1;
    } else if (vendor_name == "Juniper") {
        return 2;
    } else {
        return 0;
    }
}

int connectionInfo::getDeviceId(int vendor_id, std::string device_name) {
    chronicleAssert(vendor_id > 0, 109, "connectionInfo::getDeviceId", "No vendor ID found for " + device_name);
    if (device_name ==          "ENCS 5100")            return 1;
    else if (device_name ==     "C1700")                return 2;
    else if (device_name ==     "C7200")                return 2;
    else if (device_name ==     "Catalyst-8000V")       return 3;
    else if (device_name ==     "CSR1000V")             return 4;
    else return 0;
}

connectionInfo getConnectionInfo(std::string section) {
    if (!chronicleConfig.HasSection(section)) throwChronicleException(112,"getConnectionInfo","(" + section + ")");
    connectionInfo ci;
    int vendor_id = ci.getVendorId(chronicleConfig.Get(section, "Vendor", "UNKNOWN"));
        chronicleAssert(vendor_id > 0, 101, "connectionInfo::getDeviceId", "Vendor ID not found for " + section);
    int device_id = ci.getDeviceId(vendor_id, chronicleConfig.Get(section, "DeviceName", "UNKNOWN"));
        chronicleAssert(device_id > 0, 101, "connectionInfo::getDeviceId", "Device ID not found for " + section);
    int verbosity = chronicleConfig.GetInteger(section, "SshVerbosity", CHRONICLE_CONFIG_DEFUALT_VERBOSITY);
        chronicleAssert(4 >= verbosity && verbosity >= 0, 101, "connectionInfo::getDeviceId", "SshVerbosity can only be between 0-4");
    ci.vendor =                 vendor_id;
    ci.device =                 device_id;
    ci.user =                   chronicleConfig.Get(section, "User", CHRONICLE_CONFIG_DEFUALT_USER);
    ci.password =               chronicleConfig.Get(section, "Password", "UNKNOWN");
    ci.host =                   chronicleConfig.Get(section, "Host", "UNKNOWN");
    ci.port =                   chronicleConfig.GetInteger(section, "Port", CHRONICLE_CONFIG_DEFUALT_PORT);
    ci.kex_methods =            chronicleConfig.Get(section, "KexMethods", CHRONICLE_CONFIG_DEFUALT_KEX_METHODS);
    ci.hostkey_algorithms =     chronicleConfig.Get(section, "HostKeyAlgorithms", CHRONICLE_CONFIG_DEFUALT_HOSTKEYS);
    ci.verbosity =              verbosity;

    return ci;
}