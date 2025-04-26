#ifndef CHRONICLE_DEVICE_FACTORY_H
#define CHRONICLE_DEVICE_FACTORY_H
#include <vector>
#include <string>

/* This file contains the stuct for each device operation, per device, per vendor */







// Cisco devices
#define CISCO_ID 1

// Routers
#define ENCS_5100 1
#define C1700 2
#define C7200 3
#define CATALYST_8000V 4
#define CSR1000V 5
#define LINUX_TEST 80






// Juniper devices
#define JUNIPER_ID 2









struct deviceOperations {
    public:
        deviceOperations populateDeviceOperations(int device_id, int vendor_id);

        // Operations:
        std::vector<std::string> getConfig;
        std::vector<std::string> getInterfaces;

    private:
        void pushCommand(std::vector<std::string>& operation, std::string command);
};
#endif // CHRONICLE_DEVICE_FACTORY_H