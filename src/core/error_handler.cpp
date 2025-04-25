#include "core/error_handler.hpp"

std::string getErrorMsg(int internal_exit_code) {
    switch (internal_exit_code) {
        case 100: return "Unknown core level error";
        case 101: return "No password provided";
        case 102: return "No user provided";
        case 103: return "No host provided";
        case 104: return "Could not connect to host";
        case 105: return "Wrong credentials";
        case 106: return "Could not find .ini file in /etc/chronicle/";
        case 107: return "Parse error, please check your .ini file";
        case 108: return "NX config, use chronicleConfig first";
        case 109: return "Assertion failed, core level error";
        case 110: return "Failed creating an SSH session";
        case 111: return "Unknown ssh error";
        default: return "Unknown error.";
    }
}

ChronicleException::ChronicleException(int code, const std::string& message)
    : std::runtime_error(message), code(code), sub_message_("") {}

ChronicleException::ChronicleException(int code, const std::string& message, const std::string& subMessage)
    : std::runtime_error(message), code(code), sub_message_(subMessage) {}

std::string ChronicleException::getDetailedInfo() const {
    std::stringstream ss;
    ss << getErrorMsg(code);
    if (!sub_message_.empty()) {
        ss << " -> " << sub_message_;
    }
    return ss.str();
}

// Implementation of the function to throw the exception
void throwChronicleException(int internal_exit_code, const std::string& sub_message) {
    throw ChronicleException(internal_exit_code, getErrorMsg(internal_exit_code), sub_message);
}

void chronicleAssert(bool statement, int internal_exit_code, const std::string& sub_message) {
    if (!statement) {
        throwChronicleException(internal_exit_code, sub_message);
    }
}