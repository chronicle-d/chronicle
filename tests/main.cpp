#include "core/ssh.hpp"
#include "core/config.hpp"
#include "core/error_handler.hpp"
#include <vector>

using namespace std;

std::vector<std::string> getDirContents(std::string unit_name, std::string dir_path, INIReader* chronicleConfig) {
    Ssh ssh;

    if (!(&chronicleConfig)) {
        readChronicleConfig(CHRONICLE_CONFIG);
    }

    // Gather info
    connectionInfo ci;
    int vendor_id = ci.getVendorId(chronicleConfig->Get(unit_name, "Vendor", "UNKNOWN")); chronicleAssert(vendor_id > 0);
    int device_id = ci.getDeviceId(vendor_id, chronicleConfig->Get(unit_name, "DeviceName", "UNKNOWN")); chronicleAssert(device_id > 0);
    ci.vendor =         vendor_id;
    ci.device =         device_id;
    ci.user =           chronicleConfig->Get(unit_name, "User", CHRONICLE_CONFIG_DEFUALT_USER);
    ci.password =       chronicleConfig->Get(unit_name, "Password", "UNKNOWN");
    ci.host =           chronicleConfig->Get(unit_name, "Host", "UNKNOWN");
    ci.port =           chronicleConfig->GetInteger(unit_name, "Port", CHRONICLE_CONFIG_DEFUALT_PORT);

    // Get list of items in directory
    ssh_session session = ssh.startSession(ci);
    std::string command = "ls " + dir_path;
    std::vector<std::string> dir_items;
    int rc = ssh.executeCommand(command.c_str(), session, dir_items);

    if (rc < 0) {
        ssh.endSession(session);
        throwChronicleException(111, "While using " + command);
    }

    ssh.endSession(session);

    return dir_items;
}

int main() {
    readChronicleConfig(CHRONICLE_CONFIG);
    std::vector<std::string> dir_items = getDirContents("alum-server", "/home/", &chronicleConfig);
    for (auto i : dir_items) {
        std::cout << "Item: " << i << std::endl;
    }
    return 0;
}