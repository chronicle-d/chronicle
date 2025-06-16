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
    1xxxx:          MongoDB error.
    15xxx:          ChronicleDB error.

*/

// Core internal errors
#define CHRONICLE_ERROR_UNKNOWN_CORE_ERROR          100
#define CHRONICLE_ERROR_ASSERTION_FAILED            101

// SSH errors
#define CHRONICLE_ERROR_SSH_UNKNOWN                 200
#define CHRONICLE_ERROR_SSH_CLOSED_REMOTE           201
#define CHRONICLE_ERROR_SSH_SESSION_FAILED          202
#define CHRONICLE_ERROR_SSH_CONNECTION_FAILED       203
#define CHRONICLE_ERROR_SSH_COMMAND_FAILED          204

// Device factory
#define CHRONICLE_ERROR_DEVICE_FACTORY_FAILED       300
#define CHRONICLE_ERROR_INVALID_DEVICE_ID           301
#define CHRONICLE_ERROR_INVALID_VENDOR_ID           302
#define CHRONICLE_ERROR_LOAD_DEVICE_MAP_FAILED      303

// MongoDB errors
#define CHRONICLE_ERROR_MONGO_UNKNOWN               10000
#define CHRONICLE_ERROR_MONGO_CONNECT_TO_DB         10001
#define CHRONICLE_ERROR_MONGO_INSERT_FAILED         10002
#define CHRONICLE_ERROR_MONGO_UPDATE_FAILED         10003
#define CHRONICLE_ERROR_MONGO_DUPLICATE             10004
#define CHRONICLE_ERROR_MONGO_DOCUMENT_NOT_FOUND    10005
#define CHRONICLE_ERROR_MONGO_DELETE_DOCUMENT       10006

// ChronicleDB logic-level errors
#define CHRONICLE_ERROR_CHRONICLE_DB_ADD_FAILED     15000
#define CHRONICLE_ERROR_CHRONICLE_DB_MODIFY_FAILED  15001
#define CHRONICLE_ERROR_CHRONICLE_DB_DELETE_FAILED  15002
#define CHRONICLE_ERROR_CHRONICLE_DB_NX_DOCUMENT    15003

std::string getErrorMsg(int internal_exit_code = CHRONICLE_ERROR_UNKNOWN_CORE_ERROR);

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

void throwChronicleException(int internal_exit_code = CHRONICLE_ERROR_UNKNOWN_CORE_ERROR, const std::string& details = "", const std::string& function = "", int line = -1, const std::string& filename = "");

void chronicleAssert(bool statement, int internal_exit_code = CHRONICLE_ERROR_ASSERTION_FAILED, const std::string& function = "", const std::string& details = "");

#endif // CHRONICLE_ERROR_HANDLER_HPP
