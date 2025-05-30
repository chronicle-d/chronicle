#ifndef CHRONICLE_ERROR_HANDLER_HPP
#define CHRONICLE_ERROR_HANDLER_HPP
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#define THROW_CHRONICLE_EXCEPTION(code, details) \
    throwChronicleException(code, details, __func__, __LINE__, __FILE__)

/*
    Error codes:
    ------------
    1XX:            Internal core error. (within C++)
    2XX:            SSH error.
    3xx:            Device factory error.
    1xxxx:          Configuration error

*/
std::string getErrorMsg(int internal_exit_code = 100);

class ChronicleException : public std::runtime_error {
public:
    ChronicleException(int code, const std::string& message, const std::string& function, const std::string& details, const std::string& file, int line);

    // Accessors
    int getCode() const;
    std::string getFunction() const;
    std::string getDetails() const;
    std::string getFile() const;
    int getLine() const;
    const char* what() const noexcept override;
    
private:
    int code_;
    std::string function_name_;
    std::string details_;
    std::string full_message_;
    std::string file_;
    int line_;
};

void throwChronicleException(int internal_exit_code = 100, const std::string& details = "", const std::string& function = "", int line = -1, const std::string& filename = "");

void chronicleAssert(bool statement, int internal_exit_code = 101, const std::string& function = "", const std::string& details = "");

#endif // CHRONICLE_ERROR_HANDLER_HPP
