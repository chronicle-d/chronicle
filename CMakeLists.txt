cmake_minimum_required(VERSION 3.15...3.29)
project(chronicle LANGUAGES C CXX)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/3party/inih
    ${CMAKE_SOURCE_DIR}/3party/inih/cpp
    /usr/include/libssh
)

set(PYBIND11_FINDPYTHON ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
find_package(pybind11 CONFIG REQUIRED)
find_package(libssh CONFIG REQUIRED)

find_library(LIBSSH_LIBRARY NAMES ssh PATHS /usr/lib/x86_64-linux-gnu /usr/lib /usr/local/lib REQUIRED)
message(STATUS "Found LIBSSH_LIBRARY: ${LIBSSH_LIBRARY}")

add_library(chronicle_core STATIC
    src/core/error_handler.cpp
    src/core/device_factory.cpp
)
set_target_properties(chronicle_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(chronicle_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

pybind11_add_module(chronicle
    src/core/chronicle.cpp
    src/core/error_handler.cpp
    src/core/ssh.cpp
    src/core/config.cpp
    src/core/device_factory.cpp
    src/bindings/chronicle.cpp
    src/bindings/devices.cpp
    3party/inih/ini.c
    3party/inih/cpp/INIReader.cpp
)

set_source_files_properties(
    3party/inih/ini.c
    PROPERTIES
    LANGUAGE C
)

target_link_libraries(chronicle PRIVATE ${LIBSSH_LIBRARY})

set_target_properties(chronicle
    PROPERTIES
    OUTPUT_NAME "chronicle"
    SUFFIX ".so"
)

install(TARGETS chronicle DESTINATION .)



## DEVICES ##

#### CISCO ####
# Catalyst 8000V
add_library(catalyst8000v SHARED
    src/deviceMaps/Cisco/CATALYST_8000V.cpp
)

target_include_directories(catalyst8000v PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(catalyst8000v PRIVATE chronicle_core)

set_target_properties(catalyst8000v
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/devices/Cisco
    PREFIX ""
    OUTPUT_NAME "CATALYST_8000V"
    SUFFIX ".cld"
)

install(TARGETS catalyst8000v DESTINATION bin/devices/Cisco)