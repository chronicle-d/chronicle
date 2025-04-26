#ifndef CHRONICLE_SSH_H
#define CHRONICLE_SSH_H
#include "core/config.hpp"
#include "libssh/libssh.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <errno.h>
/*

    # ssh.hpp
    This file is a spesfic wrapper for ssh inteded to serve only chronicles needs, it is not a full on wrapper.

*/

class Ssh {
    public:
        ssh_session startSession(connectionInfo ci);
        void endSession(ssh_session session);
        int executeCommand(const char *command, ssh_session session, std::vector<std::string> &output_lines);
    private:
        std::string verifyKnownHost(ssh_session session);
};

#endif // CHRONICLE_SSH_H