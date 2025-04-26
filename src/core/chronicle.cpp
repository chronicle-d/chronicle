#include "core/chronicle.hpp"

std::vector<std::string> getDirContents(std::string dir_path, connectionInfo ci) {
    Ssh ssh;

    if (!(&chronicleConfig)) {
        readChronicleConfig(CHRONICLE_CONFIG);
    }

    // Get list of items in directory
    ssh_session session = ssh.startSession(ci);
    std::string command = "ls " + dir_path;
    std::vector<std::string> dir_items;
    int rc = ssh.executeCommand(command.c_str(), session, dir_items);

    if (rc < 0) {
        ssh.endSession(session);
        ChronicleException(111, "While using " + command);
    }

    ssh.endSession(session);

    return dir_items;
}