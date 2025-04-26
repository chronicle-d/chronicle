#include "core/config.hpp"
#include "core/device_factory.hpp"

INIReader chronicleConfig("");
int readChronicleConfig(std::string config_path) {
    chronicleConfig = INIReader(config_path);
    int parse_error = chronicleConfig.ParseError();

    if (parse_error < 0) {
        throwChronicleException(106, "readChronicleConfig", "(file: " + config_path + ")");
    } else if (parse_error > 0) {
        throwChronicleException(107, "readChronicleConfig", "(file: " + config_path + ")");
    } else {
        return 0;
    }

    return 0;
}

int connectionInfo::getVendorId(std::string vendor_name) {
    if (vendor_name == "Cisco") {
        return CISCO_ID;
    } else if (vendor_name == "Juniper") {
        return JUNIPER_ID;
    } else {
        return 0;
    }
}

int connectionInfo::getDeviceId(int vendor_id, std::string device_name) {
    chronicleAssert(vendor_id > 0, 109, "connectionInfo::getDeviceId", "No vendor ID found for " + device_name);

    switch (vendor_id) {
        case CISCO_ID: {
            if (device_name ==          "ENCS_5100")            return ENCS_5100;
            else if (device_name ==     "C1700")                return C1700;
            else if (device_name ==     "C7200")                return C7200;
            else if (device_name ==     "CATALYST_8000V")       return CATALYST_8000V;
            else if (device_name ==     "CSR1000V")             return CSR1000V;
            else if (device_name ==     "LINUX_TEST")           return LINUX_TEST;
            else return 0;
            break;
        }

        case JUNIPER_ID: {
            // Not implemented
            return 0;
        }
    }

    return 0;
}

connectionInfo getConnectionInfo(std::string section) {
    if (!chronicleConfig.HasSection(section)) throwChronicleException(112,"getConnectionInfo","(" + section + ")");
    connectionInfo ci;
    int vendor_id = ci.getVendorId(chronicleConfig.Get(section, "Vendor", "UNKNOWN"));
        chronicleAssert(vendor_id > 0, 101,
            "connectionInfo::getDeviceId", "Vendor ID not found for " + chronicleConfig.Get(section, "Vendor", "UNKNOWN") + " (DEVICE: " + section +")");
    int device_id = ci.getDeviceId(vendor_id, chronicleConfig.Get(section, "DeviceName", "UNKNOWN"));
        chronicleAssert(device_id > 0, 101,
            "connectionInfo::getDeviceId", "Device ID not found for " + chronicleConfig.Get(section, "DeviceName", "UNKNOWN") + " (DEVICE: " + section +")");
    int verbosity = chronicleConfig.GetInteger(section, "SshVerbosity", CHRONICLE_CONFIG_DEFUALT_VERBOSITY);
        chronicleAssert(4 >= verbosity && verbosity >= 0, 101, "connectionInfo::getDeviceId", "SshVerbosity can only be between 0-4 (DEVICE: " + section +")");
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