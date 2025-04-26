#include "core/error_handler.hpp"

std::string getErrorMsg(int internal_exit_code) {
    switch (internal_exit_code) {
        case 100: return "Unknown core level error";
        case 101: return "Configuration error";
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
        case 112: return "No such section in configuration file";
        case 113: return "Reomte error";
        default: return "Unknown error.";
    }
}

ChronicleException::ChronicleException(int code, const std::string& message, const std::string& function, const std::string& details)
    : std::runtime_error(message), code_(code), function_name_(function), details_(details)
{
    std::ostringstream oss;
    oss << "[ChronicleError: " << code_ << "]";
    if (!function_name_.empty()) {
        oss << "[" << function_name_ << "] ";
    }
    oss << message;
    if (!details_.empty()) {
        oss << " => " << details_;
    }
    full_message_ = oss.str();
}

void throwChronicleException(int internal_exit_code, const std::string& function, const std::string& details) {
    throw ChronicleException(internal_exit_code, getErrorMsg(internal_exit_code), function, details);
}

void chronicleAssert(bool statement, int internal_exit_code, const std::string& function, const std::string& details) {
    if (!statement) {
        throwChronicleException(internal_exit_code, function, details);
    }
}