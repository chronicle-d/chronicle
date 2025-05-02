#ifndef CHRONICLE_DEVICE_FACTORY_H
#define CHRONICLE_DEVICE_FACTORY_H
#include <vector>
#include <string>

/* ---------- Vendor & Device IDs ---------- */

// Cisco vendor ID
#define CISCO_ID 1
#define ENCS_5100 1
#define C1700     2
#define C7200     3
#define CATALYST_8000V 4
#define CSR1000V  5
#define LINUX_TEST 80

// Juniper vendor ID
#define JUNIPER_ID 2

/* ---------- Data Structures ---------- */

struct OperationMap {
    std::string command;
    int skip_head;
    int skip_tail;
    std::string err_msg;
};

struct deviceOperations {
    std::vector<OperationMap> getConfig;
    std::vector<OperationMap> getInterfaces;

    void pushCommand(std::vector<OperationMap>& list, std::string command, int skip_head, int skip_tail, std::string err_msg);
};

/* ---------- Required Plugin Exports ---------- */

extern "C" deviceOperations* createDeviceOperations(int device_id, int vendor_id);
extern "C" const char* getDeviceDetails();

#endif // CHRONICLE_DEVICE_FACTORY_H