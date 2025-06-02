#include "database_handler.hpp"

MongoDB mdb;

void ChronicleDB::addDevice(
  // General
  const std::string& deviceNickname,
  const std::string& deviceName,
  const std::string& vendor,

  // SSH
  const std::string& user,
  const std::string& password,
  const std::string& host,
  const int& port,
  const int& sshVerbosity,
  const std::string& kexMethods,
  const std::string& hostkeyAlgorithms
) {
  bsoncxx::builder::basic::document deviceData;

  deviceData.append(
    bsoncxx::builder::basic::kvp("device", bsoncxx::builder::basic::make_document(
      bsoncxx::builder::basic::kvp("name", deviceNickname),
      bsoncxx::builder::basic::kvp("deviceName", deviceName),
      bsoncxx::builder::basic::kvp("vendorName", vendor)
    ))
  );

  deviceData.append(
    bsoncxx::builder::basic::kvp("ssh", bsoncxx::builder::basic::make_document(
      bsoncxx::builder::basic::kvp("user", user),
      bsoncxx::builder::basic::kvp("password", password),
      bsoncxx::builder::basic::kvp("host", host),
      bsoncxx::builder::basic::kvp("port", port),
      bsoncxx::builder::basic::kvp("kexMethods", kexMethods),
      bsoncxx::builder::basic::kvp("hostkeyAlgorithms", hostkeyAlgorithms)
    ))
  );

  try {
    mdb.insertDocument(mdb.devices_c, deviceData.view());
  } catch (const ChronicleException& e) {

    std::string fullMessage;

    if (e.getCode() == CHRONICLE_ERROR_MONGO_DUPLICATE) {
      fullMessage = "Device named '" + deviceNickname + "' exists already.";
      THROW_CHRONICLE_EXCEPTION(CHRONICLE_ERROR_CHRONICLE_DB_ADD_FAILED, fullMessage);
    }

    fullMessage =
      "Chronicle exception:\n"
      "ChronicleCode: " + std::to_string(e.getCode()) + "\n"
      "Details: " + e.getDetails();
    THROW_CHRONICLE_EXCEPTION(CHRONICLE_ERROR_CHRONICLE_DB_ADD_FAILED, fullMessage);
  } catch (const std::exception& e) {
    THROW_CHRONICLE_EXCEPTION(CHRONICLE_ERROR_CHRONICLE_DB_ADD_FAILED, e.what());
  }
}

void ChronicleDB::modifyDevice(
  const std::string& deviceNickname,

  // Device
  std::optional<std::string> deviceName,
  std::optional<std::string> vendor,

  // SSH fields
  std::optional<std::string> user,
  std::optional<std::string> password,
  std::optional<std::string> host,
  std::optional<int> port,
  std::optional<int> sshVerbosity,
  std::optional<std::string> kexMethods,
  std::optional<std::string> hostkeyAlgorithms
) {
  
  bsoncxx::builder::basic::document updateDoc;
  
  // Device fields
  if (deviceName)         updateDoc.append(bsoncxx::builder::basic::kvp("device.deviceName", *deviceName));
  if (vendor)             updateDoc.append(bsoncxx::builder::basic::kvp("device.vendorName", *vendor));

  // SSH fields
  if (user)               updateDoc.append(bsoncxx::builder::basic::kvp("ssh.user", *user));
  if (password)           updateDoc.append(bsoncxx::builder::basic::kvp("ssh.password", *password));
  if (host)               updateDoc.append(bsoncxx::builder::basic::kvp("ssh.host", *host));
  if (port)               updateDoc.append(bsoncxx::builder::basic::kvp("ssh.port", *port));
  if (sshVerbosity)       updateDoc.append(bsoncxx::builder::basic::kvp("ssh.sshVerbosity", *sshVerbosity));
  if (kexMethods)         updateDoc.append(bsoncxx::builder::basic::kvp("ssh.kexMethods", *kexMethods));
  if (hostkeyAlgorithms)  updateDoc.append(bsoncxx::builder::basic::kvp("ssh.hostkeyAlgorithms", *hostkeyAlgorithms));

  if (updateDoc.view().empty()) {
    THROW_CHRONICLE_EXCEPTION(CHRONICLE_ERROR_CHRONICLE_DB_MODIFY_FAILED, "No fields provided to modify.");
  }

  try {
    mdb.updateDocument(mdb.devices_c, deviceNickname, updateDoc.view());
  } catch (const ChronicleException& e) {

    std::string fullMessage;

    if (e.getCode() == CHRONICLE_ERROR_MONGO_DOCUMENT_NOT_FOUND) {
      fullMessage = "Device " + deviceNickname + " not found";
      THROW_CHRONICLE_EXCEPTION(CHRONICLE_ERROR_CHRONICLE_DB_MODIFY_FAILED, fullMessage);
    }

    fullMessage =
      "Chronicle exception:\n"
      "ChronicleCode: " + std::to_string(e.getCode()) + "\n"
      "Details: " + e.getDetails();
    THROW_CHRONICLE_EXCEPTION(CHRONICLE_ERROR_CHRONICLE_DB_MODIFY_FAILED, fullMessage);
  } catch (const std::exception& e) {
    THROW_CHRONICLE_EXCEPTION(CHRONICLE_ERROR_CHRONICLE_DB_MODIFY_FAILED, e.what());
  }
}

void ChronicleDB::deleteDevice(const std::string& deviceNickname) {
  try {
    mdb.deleteDocument(mdb.devices_c, deviceNickname);
  } catch (const ChronicleException& e) {
    std::string fullMessage;

    if (e.getCode() == CHRONICLE_ERROR_MONGO_DOCUMENT_NOT_FOUND) {
      fullMessage = "Device " + deviceNickname + " not found";
      THROW_CHRONICLE_EXCEPTION(CHRONICLE_ERROR_CHRONICLE_DB_DELETE_FAILED, fullMessage);
    }

    fullMessage =
      "Chronicle exception:\n"
      "ChronicleCode: " + std::to_string(e.getCode()) + "\n"
      "Details: " + e.getDetails();
    THROW_CHRONICLE_EXCEPTION(CHRONICLE_ERROR_CHRONICLE_DB_DELETE_FAILED, fullMessage);
  } catch (const std::exception& e) {
    THROW_CHRONICLE_EXCEPTION(CHRONICLE_ERROR_CHRONICLE_DB_DELETE_FAILED, e.what());
  }
}
