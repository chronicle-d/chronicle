#include "core/config.hpp"
#include "database_handler.hpp"



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

connectionInfo getConnectionInfo(std::string deviceNickname) {
  connectionInfo ci;

  /* Fetch device settings */
  ChronicleDB cdb;

  auto deviceSettings = cdb.getDeviceBson(deviceNickname);
  auto ssh = deviceSettings["ssh"].get_document().view();
  auto device = deviceSettings["device"].get_document().view();

  /* Initialize */
  std::string vendorName = std::string(device["vendorName"].get_string().value);
  std::string deviceName = std::string(device["deviceName"].get_string().value);;

  ci.vendorName         = vendorName;
  ci.deviceName         = deviceName;
  ci.vendor             = ci.getVendorId(vendorName);
  ci.device             = ci.getDeviceId(ci.vendor, deviceName);
  ci.user               = std::string(ssh["user"].get_string().value);
  ci.password           = std::string(ssh["password"].get_string().value);
  ci.host               = std::string(ssh["host"].get_string().value);
  ci.port               = ssh["port"].get_int32();
  ci.verbosity          = ssh["verbosity"].get_int32();
  ci.kex_methods        = std::string(ssh["kexMethods"].get_string().value);
  ci.hostkey_algorithms = std::string(ssh["hostkeyAlgorithms"].get_string().value);

  return ci;
}

chronicleSettings getChronicleSettings() {
  chronicleSettings cs;

  /* Fetch chronicle settings */
  ChronicleDB cdb;

  auto settings = cdb.getSettingsBson();
  auto ssh = settings["ssh"].get_document().view();

  cs.ssh_idle_timeout   = ssh["sshIdleTimeout"].get_int32();
  cs.ssh_total_timeout  = ssh["sshTotalTimeout"].get_int32();

  return cs;
}
