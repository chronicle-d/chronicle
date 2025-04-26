#include "core/chronicle.hpp"
#include "core/device_factory.hpp"

std::vector<std::string> getConfig(connectionInfo ci) {
    deviceOperations devOps = devOps.populateDeviceOperations(ci.device, ci.vendor);

    Ssh ssh;
    ssh_session session = ssh.startSession(ci);
    std::vector<std::string> output;

    for (auto cmd : devOps.getConfig) {
        int rc = ssh.executeCommand(cmd.c_str(), session, output);
        if (rc != 0) {
            ssh.endSession(session);
            throwChronicleException(201,"getConfig","While using: `" + cmd + "`, EXIT CODE: " + std::to_string(rc));
        }
    }

    ssh.endSession(session);

    return output;
}

std::vector<std::string> getInterfaces(connectionInfo ci) {
    deviceOperations devOps = devOps.populateDeviceOperations(ci.device, ci.vendor);

    Ssh ssh;
    ssh_session session = ssh.startSession(ci);
    std::vector<std::string> output;

    for (auto cmd : devOps.getInterfaces) {
        int rc = ssh.executeCommand(cmd.c_str(), session, output);
        if (rc != 0) {
            ssh.endSession(session);
            throwChronicleException(201,"getInterfaces","While using: `" + cmd + "`, EXIT CODE: " + std::to_string(rc));
        }
    }

    ssh.endSession(session);

    return output;
}