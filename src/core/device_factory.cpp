#include "core/device_factory.hpp"
#include "core/error_handler.hpp"

void deviceOperations::pushCommand(std::vector<std::string>& operation, std::string command) {
    operation.push_back(command);
}

deviceOperations deviceOperations::populateDeviceOperations(int device_id, int vendor_id) {
    deviceOperations devOps;

    if (vendor_id == CISCO_ID) {

        switch (device_id) {



            case ENCS_5100: {
                /* getConfig */
                devOps.pushCommand(devOps.getConfig, "terminal length 0");
                devOps.pushCommand(devOps.getConfig, "show startup-config");

                /* getInterfaces */
                devOps.pushCommand(devOps.getInterfaces, "show ip interface brief");
                break;
            }

            case C1700: {
                /* getConfig */
                devOps.pushCommand(devOps.getConfig, "terminal length 0");
                devOps.pushCommand(devOps.getConfig, "show startup-config");

                /* getInterfaces */
                devOps.pushCommand(devOps.getInterfaces, "show ip interface brief");
                break;
            }

            case C7200: {
                /* getConfig */
                devOps.pushCommand(devOps.getConfig, "terminal length 0");
                devOps.pushCommand(devOps.getConfig, "show startup-config");

                /* getInterfaces */
                devOps.pushCommand(devOps.getInterfaces, "show ip interface brief");
                break;
            }

            case CATALYST_8000V: {
                /* getConfig */
                devOps.pushCommand(devOps.getConfig, "terminal length 0");
                devOps.pushCommand(devOps.getConfig, "show startup-config");

                /* getInterfaces */
                devOps.pushCommand(devOps.getInterfaces, "show ip interface brief");
                break;
            }

            case CSR1000V: {
                /* getConfig */
                devOps.pushCommand(devOps.getConfig, "terminal length 0");
                devOps.pushCommand(devOps.getConfig, "show startup-config");

                /* getInterfaces */
                devOps.pushCommand(devOps.getInterfaces, "show ip interface brief");
                break;
            }

            case LINUX_TEST: {
                /* getConfig */
                devOps.pushCommand(devOps.getConfig, "cat ~/.bashrc");

                /* getInterfaces */
                devOps.pushCommand(devOps.getInterfaces, "ip a");
                break;
            }

        default:
            THROW_CHRONICLE_EXCEPTION(114, "No such device ID for Cisco devices. (Device ID: " + std::to_string(device_id) + ")");
            break;
        }









    } else if (vendor_id == JUNIPER_ID) {
        // Not implemented yet











    } else {
        THROW_CHRONICLE_EXCEPTION(114, "No such vendor ID. (" + std::to_string(vendor_id) + ")");
    }

    return devOps;
}