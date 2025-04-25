#include "core/config.hpp"

INIReader chronicleConfig("");
int readChronicleConfig(std::string config_path) {
    chronicleConfig = INIReader(config_path);

    if (chronicleConfig.ParseError() < 0) {
        throwChronicleException(107);
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
    chronicleAssert(vendor_id > 0);
    if (device_name == "ENCS 5100") {
        return 1;
    } else if (device_name == "ENCS 5400") {
        return 2;
    } else {
        return 0;
    }
}