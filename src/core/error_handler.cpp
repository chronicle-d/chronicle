#include "core/error_handler.hpp"

std::string getErrorMsg(int internal_exit_code) {
    switch (internal_exit_code) {
        // Internal errors
        case 100: return "Unknown core level error";
        case 101: return "Assertion failed, core level error";

        // SSH errors
        case 200: return "Unknown ssh error";
        case 201: return "Connection closed by remote";
        case 202: return "Failed creating an SSH session";
        case 203: return "Could not connect to host";

        // Device factory
        case 300: return "Error while getting device operations";

        // Configuration errors
        case 10000: return "Configuration error";
        case 10001: return "Parse error, please check your .ini file";
        case 10002: return "No such section in configuration file";
        case 10003: return "Can't load INI file (missing?)";
        default: return "Unknown error.";
    }
}

ChronicleException::ChronicleException(int code, const std::string& message, const std::string& function, const std::string& details, const std::string& file, int line)
    : std::runtime_error(message), code_(code), function_name_(function), details_(details), file_(file), line_(line)
{
    std::ostringstream oss;
    oss << "[ChronicleError: " << code_ << "]";

    if (!file_.empty())
        oss << "[" << file_ << ":" << line_ << "]";

    if (!function_name_.empty())
        oss << "[" << function_name_ << "]";

    oss << ": ";

    oss << message;
    if (!details_.empty())
        oss << " => " << details_;

    full_message_ = oss.str();
}

void throwChronicleException(int internal_exit_code, const std::string& details, const std::string& function, int line, const std::string& filename) {
    throw ChronicleException(internal_exit_code, getErrorMsg(internal_exit_code), function, details, filename, line);
}

void chronicleAssert(bool statement, int internal_exit_code, const std::string& function, const std::string& details) {
    if (!statement) {
        throwChronicleException(internal_exit_code, details, function);
    }
}