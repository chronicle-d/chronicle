#include "core/device_factory.hpp"

void deviceOperations::pushCommand(std::vector<OperationMap>& operation, std::string command, int skip_head, int skip_tail, std::string err_msg) {
    OperationMap opMap;
    opMap.command = command;
    opMap.skip_head = skip_head;
    opMap.skip_tail = skip_tail;
    opMap.err_msg = err_msg;

    operation.push_back(opMap);
}