#include "core/chronicle.hpp"

std::vector<std::string> getConfig(connectionInfo ci) {
    Ssh ssh;
    ssh_session session = ssh.startSession(ci);

    std::vector<std::string> config, output, commands = {
        "terminal length 0",
        "show startup-config"
    };

    for (auto cmd : commands) {
        std::cout << "EXECUTING: " << cmd << std::endl << std::flush;
        int rc = ssh.executeCommand(cmd.c_str(), session, output);
        if (rc != 0) {
            ssh.endSession(session);
            throwChronicleException(113,"getConfig","While using: `" + cmd + "`, EXIT CODE: " + std::to_string(rc));
        }
    }

    ssh.endSession(session);

    return output;
}