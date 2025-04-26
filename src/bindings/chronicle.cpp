#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "core/config.hpp"
#include "core/chronicle.hpp"

/*
    **To future Noam:**
    --------------------
    This is not that tough, check [this](https://pybind11.readthedocs.io/en/stable/) out once a binding is needed.
*/

namespace py = pybind11;

PYBIND11_MODULE(chronicle, m) {
    m.doc() = "Chronicle";

    /* config.cpp */
    // readChronicleConfig
    m.def("readChronicleConfig", &readChronicleConfig, "A function that reads the configuration file of Chronicle.");
    py::class_<INIReader>(m, "Config")
        .def("Get", &INIReader::Get)
        .def("GetInteger", &INIReader::GetInteger)
        .def("GetBoolean", &INIReader::GetBoolean);
    m.attr("config") = py::cast(&chronicleConfig, py::return_value_policy::reference);
    m.def("getConnectionInfo", &getConnectionInfo, "A function to create the connectionInfo struct for a given section.");

    // connectionInfo
    py::class_<connectionInfo>(m, "connectionInfo")
        .def(py::init<>())
        .def_readwrite("vendor", &connectionInfo::vendor)
        .def_readwrite("device", &connectionInfo::device)
        .def_readwrite("user", &connectionInfo::user)
        .def_readwrite("password", &connectionInfo::password)
        .def_readwrite("host", &connectionInfo::host)
        .def_readwrite("port", &connectionInfo::port)
        .def_readwrite("kex_methods", &connectionInfo::kex_methods)
        .def_readwrite("hostkey_algorithms", &connectionInfo::hostkey_algorithms)
        .def_readwrite("verbosity", &connectionInfo::verbosity);

    // Chronicle
    m.def("getDirContents", &getDirContents, "Gets the directory contents of a give directory in a given unit.");
}