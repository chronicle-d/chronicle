#ifndef CHRONICLE_ERROR_HANDLER_HPP
#define CHRONICLE_ERROR_HANDLER_HPP
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

std::string getErrorMsg(int internal_exit_code);

class ChronicleException : public std::runtime_error {
    public:
        ChronicleException(int code, const std::string& message);
        ChronicleException(int code, const std::string& message, const std::string& subMessage);
    
        int getCode() const { return code; }
        const char* what() const noexcept override { return full_message_.c_str(); }
    
    private:
        int code;
        std::string sub_message_;
        std::string full_message_;
};    

void throwChronicleException(int internal_exit_code, const std::string& sub_message = "");

void chronicleAssert(bool statement, int internal_exit_code = 109, const std::string& sub_message = "");

#endif // CHRONICLE_ERROR_HANDLER_HPP