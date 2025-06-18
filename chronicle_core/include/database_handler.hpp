#ifndef CHRONICLE_DATABASE_HANDLER_HPP
#define CHRONICLE_DATABASE_HANDLER_HPP

#include "core/config.hpp"
#include "core/mongodb.hpp"
#include "core/error_handler.hpp"
#include <optional>
#include <vector>

class ChronicleDB {
  public:
 
    void connect();

    struct MongoProjections {
      static const bsoncxx::document::view_or_value device();
      static const bsoncxx::document::view_or_value settings();
    };

    /* Global */
    void initDB() const;

    /* Chronicle settings */
    void updateSettings(
      std::optional<int> sshIdleTimeout = CHRONICLE_CONFIG_DEFAULT_SSH_IDLE_TIMEOUT,
      std::optional<int> sshTotalTimeout = CHRONICLE_CONFIG_DEFAULT_SSH_TOTAL_TIMEOUT
    ) const;
    std::string getSettings() const;

    /* Devices */
    void addDevice(
      // General
      const std::string& deviceNickname,
      const std::string& deviceName,
      const std::string& vendor,

      // SSH
      const std::string& user,
      const std::string& password,
      const std::string& host,
      const int& port = CHRONICLE_CONFIG_DEFUALT_PORT,
      const int& sshVerbosity = 0,
      const std::string& kexMethods = CHRONICLE_CONFIG_DEFUALT_KEX_METHODS,
      const std::string& hostkeyAlgorithms = CHRONICLE_CONFIG_DEFUALT_HOSTKEYS
    ) const;

    void modifyDevice(
      const std::string& deviceNickname,

      // Device
      std::optional<std::string> deviceName = std::nullopt,
      std::optional<std::string> vendor = std::nullopt,

      // SSH fields
      std::optional<std::string> user = std::nullopt,
      std::optional<std::string> password = std::nullopt,
      std::optional<std::string> host = std::nullopt,
      std::optional<int> port = std::nullopt,
      std::optional<int> sshVerbosity = std::nullopt,
      std::optional<std::string> kexMethods = std::nullopt,
      std::optional<std::string> hostkeyAlgorithms = std::nullopt
    ) const;

    void deleteDevice(const std::string& deviceNickname) const;
    std::vector<std::string> listDevices() const;
    std::string getDevice(const std::string& deviceNickname) const;

    // C++ Internal methods
    bsoncxx::document::value getDeviceBson(const std::string& deviceNickname) const;
    bsoncxx::document::value getSettingsBson() const;
    
};
#endif // CHRONICLE_DATABASE_HANDLER_HPP
