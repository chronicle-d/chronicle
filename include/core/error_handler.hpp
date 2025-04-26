#ifndef CHRONICLE_ERROR_HANDLER_HPP
#define CHRONICLE_ERROR_HANDLER_HPP
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

std::string getErrorMsg(int internal_exit_code);

class ChronicleException : public std::runtime_error {
public:
    ChronicleException(int code, const std::string& message, const std::string& function = "", const std::string& details = "");

    int getCode() const { return code_; }
    const char* what() const noexcept override { return full_message_.c_str(); }
    
private:
    int code_;
    std::string function_name_;
    std::string details_;
    std::string full_message_;
};

void throwChronicleException(int internal_exit_code, const std::string& function = "", const std::string& details = "");

void chronicleAssert(bool statement, int internal_exit_code = 109, const std::string& function = "", const std::string& details = "");

#endif // CHRONICLE_ERROR_HANDLER_HPP