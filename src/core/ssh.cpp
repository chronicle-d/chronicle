#include "core/ssh.hpp"
#include "core/config.hpp"
#include "core/chronicle.hpp"
#include "core/error_handler.hpp"

ssh_session Ssh::startSession(connectionInfo ci) {
    ssh_session session;

    session = ssh_new();
    if (session == NULL) {
        throwChronicleException(110,"Ssh::startSession",ssh_get_error(session));
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, ci.host.c_str());
    ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &ci.verbosity);
    ssh_options_set(session, SSH_OPTIONS_PORT, &ci.port);
    ssh_options_set(session, SSH_OPTIONS_KEY_EXCHANGE, ci.kex_methods.c_str());
    ssh_options_set(session, SSH_OPTIONS_HOSTKEYS, ci.hostkey_algorithms.c_str());

    int rc = ssh_connect(session);
    if (rc != SSH_OK)
    {
      throwChronicleException(104,"Ssh::startSession",ssh_get_error(session));
    }

    std::string known_host_msg = verifyKnownHost(session);
    if (known_host_msg.size() > 0) {
        endSession(session);
        throwChronicleException(110,"Ssh::startSession",known_host_msg);
    }

    rc = ssh_userauth_password(session, ci.user.c_str(), ci.password.c_str());
    if (rc != SSH_AUTH_SUCCESS) {
        endSession(session);
        throwChronicleException(110,"Ssh::startSession",ssh_get_error(session));
    }

    return session;
}

void Ssh::endSession(ssh_session session) {
    ssh_disconnect(session);
    ssh_free(session);
}

int Ssh::executeCommand(const char *command, ssh_session session, std::vector<std::string> &output_lines) {
    ssh_channel channel;
    int rc;
    char buffer[256];
    int nbytes;
    std::string total_output;

    if (!ssh_is_connected(session)) {
        endSession(session);
        throwChronicleException(111, "Ssh::executeCommand", "SSH session died before executing command: `" + std::string(command) + "`.");
    }
   
    channel = ssh_channel_new(session);
    if (channel == NULL)
        throwChronicleException(110,"Ssh::executeCommand",ssh_get_error(session));
   
    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
      ssh_channel_free(channel);
      throwChronicleException(110,"Ssh::executeCommand",ssh_get_error(session));
    }
   
    rc = ssh_channel_request_exec(channel, command);
    if (rc != SSH_OK) {
      ssh_channel_close(channel);
      ssh_channel_free(channel);
      throwChronicleException(110,"Ssh::executeCommand",ssh_get_error(session));
    }
   
    while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0) {
        total_output.append(buffer, nbytes);
    }

    if (nbytes < 0) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        throwChronicleException(110,"Ssh::executeCommand",ssh_get_error(session));
    }
   
    int remote_exit_code = ssh_channel_get_exit_status(channel);
    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);

    std::istringstream iss(total_output);
    std::string line;
    while (std::getline(iss, line)) {
        output_lines.push_back(line);
    }
   
    return remote_exit_code;
}

std::string Ssh::verifyKnownHost(ssh_session session) {
    enum ssh_known_hosts_e state;
    unsigned char *hash = NULL;
    ssh_key srv_pubkey = NULL;
    size_t hlen;
    char *p;
    int cmp;
    int rc;
 
    rc = ssh_get_server_publickey(session, &srv_pubkey);
    if (rc < 0) {
        return "Could not get server public key.";
    }
 
    rc = ssh_get_publickey_hash(srv_pubkey,
                                SSH_PUBLICKEY_HASH_SHA1,
                                &hash,
                                &hlen);
    ssh_key_free(srv_pubkey);
    if (rc < 0) {
        return "Could not get server public key hash.";
    }
 
    state = ssh_session_is_known_server(session);
    switch (state) {
        case SSH_KNOWN_HOSTS_OK:
            /* OK */
 
            break;
        case SSH_KNOWN_HOSTS_CHANGED: ssh_clean_pubkey_hash(&hash);
            return "Host key for server changed.";
        case SSH_KNOWN_HOSTS_OTHER|SSH_KNOWN_HOSTS_NOT_FOUND:
            ssh_clean_pubkey_hash(&hash);
            return "The host key for this server was not found.";
        case SSH_KNOWN_HOSTS_UNKNOWN:
            ssh_clean_pubkey_hash(&hash);
            rc = ssh_session_update_known_hosts(session);
            if (rc < 0) {
                return strerror(errno);
            }
            break;
        case SSH_KNOWN_HOSTS_ERROR:
            ssh_clean_pubkey_hash(&hash);
            return ssh_get_error(session);
    }
 
    ssh_clean_pubkey_hash(&hash);
    return "";
}