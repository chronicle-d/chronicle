#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "core/config.hpp"
#include "core/chronicle.hpp"
#include "core/error_handler.hpp"

namespace py = pybind11;

extern void bind_device_loader(py::module_&);

PYBIND11_MODULE(chronicle, m) {
    m.doc() = "Chronicle";

    // Chronicle Exception
    static py::exception<ChronicleException> chronicle_exc(m, "ChronicleException");

    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const ChronicleException& e) {
            PyObject* exc_inst = PyObject_CallFunction(chronicle_exc.ptr(), "s", e.what());
            if (exc_inst) {
                py::object obj = py::reinterpret_steal<py::object>(exc_inst);
                obj.attr("code") = e.getCode();
                obj.attr("function") = e.getFunction();
                obj.attr("details") = e.getDetails();
                obj.attr("file") = e.getFile();
                obj.attr("line") = e.getLine();
                PyErr_SetObject(chronicle_exc.ptr(), obj.ptr());
            } else {
                PyErr_SetString(PyExc_RuntimeError, "Failed to construct ChronicleException");
            }
        }
    });

    m.def("getErrorMsg", &getErrorMsg, "Returns the message of a given error code.");

    // INI config support
    m.def("readChronicleConfig", &readChronicleConfig, "Reads the Chronicle configuration file.");

    py::class_<INIReader>(m, "Config")
        .def("Get", &INIReader::Get)
        .def("GetInteger", &INIReader::GetInteger)
        .def("GetBoolean", &INIReader::GetBoolean)
        .def("Sections", &INIReader::Sections);

    m.attr("config") = py::cast(&chronicleConfig, py::return_value_policy::reference);

    m.def("getConnectionInfo", &getConnectionInfo, "Get connectionInfo for a given config section.");
    m.def("getChronicleSettings", &getChronicleSettings, "Get chronicleSettings from config.");

    // Structs
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
        .def_readwrite("verbosity", &connectionInfo::verbosity)
        .def("getVendorId", &connectionInfo::getVendorId)
        .def("getDeviceId", &connectionInfo::getDeviceId);

    py::class_<chronicleSettings>(m, "chronicleSettings")
        .def_readwrite("ssh_idle_timeout", &chronicleSettings::ssh_idle_timeout)
        .def_readwrite("ssh_total_timeout", &chronicleSettings::ssh_total_timeout);

    m.def("getConfig", &getConfig, "Returns the current device configuration.");

    bind_device_loader(m);
}
