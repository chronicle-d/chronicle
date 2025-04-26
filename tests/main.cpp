#include "core/ssh.hpp"
#include "core/config.hpp"
#include "core/error_handler.hpp"
#include <vector>

using namespace std;

std::vector<std::string> getDirContents(std::string section, std::string dir_path, INIReader* chronicleConfig) {
    Ssh ssh;

    if (!(&chronicleConfig)) {
        readChronicleConfig(CHRONICLE_CONFIG);
    }

    // Gather info
    connectionInfo ci = getConnectionInfo(section);

    // Get list of items in directory
    ssh_session session = ssh.startSession(ci);
    std::string command = "ls " + dir_path;
    std::vector<std::string> dir_items;

    try {
        int rc = ssh.executeCommand(command.c_str(), session, dir_items);

        if (rc < 0) {
            ssh.endSession(session);
            throwChronicleException(111, "While using " + command);
        }
    } catch (...) {
        ssh.endSession(session);
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
