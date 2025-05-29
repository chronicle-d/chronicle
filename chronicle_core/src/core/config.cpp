#include "core/config.hpp"
#include "core/device_factory.hpp"

INIReader chronicleConfig("");
int readChronicleConfig(std::string config_path) {
    chronicleConfig = INIReader(config_path);
    int parse_error = chronicleConfig.ParseError();

    if (parse_error < 0) {
        THROW_CHRONICLE_EXCEPTION(10003, "(file: " + config_path + ")");
    } else if (parse_error > 0) {
        THROW_CHRONICLE_EXCEPTION(10001, "(file: " + config_path + ")");
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
    chronicleAssert(vendor_id > 0, 10000, "connectionInfo::getDeviceId", "No vendor ID found for " + device_name);

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
            if (device_name ==          "VSRX")            return VSRX;
            return 0;
        }
    }

    return 0;
}

connectionInfo getConnectionInfo(std::string section) {
  if (!chronicleConfig.HasSection(section)) THROW_CHRONICLE_EXCEPTION(10002, "(" + section + ")");
  connectionInfo ci;

  /* sanitize */
  int vendor_id = ci.getVendorId(chronicleConfig.Get(section, "Vendor", ""));
      chronicleAssert(vendor_id > 0, 10000, "connectionInfo::getDeviceId",
          "Vendor ID not found for vendor `" + chronicleConfig.Get(section, "Vendor", "") + "` (DEVICE: " + section +")");

  int device_id = ci.getDeviceId(vendor_id, chronicleConfig.Get(section, "DeviceName", ""));
      chronicleAssert(device_id > 0, 10000, "connectionInfo::getDeviceId",
          "Device ID not found for device `" + chronicleConfig.Get(section, "DeviceName", "") + "` (DEVICE: " + section +")");

  int verbosity = chronicleConfig.GetInteger(section, "SshVerbosity", CHRONICLE_CONFIG_DEFUALT_VERBOSITY);
      chronicleAssert(4 >= verbosity && verbosity >= 0, 10000, "connectionInfo::getDeviceId",
          "SshVerbosity can only be between 0-4 (CURRENT VALUE: " + std::to_string(verbosity) + " DEVICE: " + section +")");

  std::string user = chronicleConfig.Get(section, "User", CHRONICLE_CONFIG_DEFUALT_USER);
      chronicleAssert(!user.empty(), 10000, "connectionInfo::getDeviceId",
          "No User provided (DEVICE: " + section +")");

  std::string password = chronicleConfig.Get(section, "Password", "");
      chronicleAssert(!password.empty(), 10000, "connectionInfo::getDeviceId",
          "No Password provided (DEVICE: " + section +")");

  std::string host = chronicleConfig.Get(section, "Host", "");
      chronicleAssert(!host.empty(), 10000, "connectionInfo::getDeviceId",
          "No Host provided (DEVICE: " + section +")");

  int port = chronicleConfig.GetInteger(section, "Port", CHRONICLE_CONFIG_DEFUALT_PORT);
      chronicleAssert(port > 0, 10000, "connectionInfo::getDeviceId",
          "Port numbers can only be positive (CURRENT VALUE: " + std::to_string(port) + " DEVICE: " + section +")");

  /* initiate */
  ci.vendor =                 vendor_id;
  ci.device =                 device_id;
  ci.user =                   user;
  ci.password =               password;
  ci.host =                   host;
  ci.port =                   port;
  ci.kex_methods =            chronicleConfig.Get(section, "KexMethods", CHRONICLE_CONFIG_DEFUALT_KEX_METHODS);
  ci.hostkey_algorithms =     chronicleConfig.Get(section, "HostKeyAlgorithms", CHRONICLE_CONFIG_DEFUALT_HOSTKEYS);
  ci.verbosity =              verbosity;

  return ci;
}

chronicleSettings getChronicleSettings() {
  chronicleSettings cs;

  if (chronicleConfig.HasSection("ChronicleSettings")) {
    cs.ssh_idle_timeout   =    chronicleConfig.GetInteger("ChronicleSettings", "SshIdleTimeout", CHRONICLE_CONFIG_DEFAULT_SSH_IDLE_TIMEOUT);
    cs.ssh_total_timeout =     chronicleConfig.GetInteger("ChronicleSettings", "SshTotalTimeout", CHRONICLE_CONFIG_DEFAULT_SSH_TOTAL_TIMEOUT);
  }

  return cs;
}
