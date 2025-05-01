#ifndef CHRONICLE_SSH_H
#define CHRONICLE_SSH_H
#include "core/config.hpp"
#include "libssh/libssh.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <errno.h>
#include <chrono>
#include <thread>
#define SSH_FLUSH_BANNER(session, channel) \
    Sss::executeCommand("", session, channel);
/*

    # ssh.hpp
    This file is a spesfic wrapper for ssh inteded to serve only chronicles needs, it is not a full on wrapper.

*/

class Ssh {
    public:
        ssh_session startSession(connectionInfo ci);
        void endSession(ssh_session session);
        std::vector<std::string> executeCommand(const char *command, ssh_session session, ssh_channel channel);
        ssh_channel startChannel(ssh_session session);
        void closeChannel(ssh_channel channel);
        void flushBanner(ssh_session session, ssh_channel channel);
    private:
        std::string verifyKnownHost(ssh_session session);
};

#endif // CHRONICLE_SSH_H