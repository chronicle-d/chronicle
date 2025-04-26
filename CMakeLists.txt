cmake_minimum_required(VERSION 3.15...3.29)
project(chronicle LANGUAGES C CXX)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/3party/inih
    ${CMAKE_SOURCE_DIR}/3party/inih/cpp
    /usr/include/libssh # Explicitly add libssh include path
)

set(PYBIND11_FINDPYTHON ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
find_package(pybind11 CONFIG REQUIRED)
find_package(libssh CONFIG REQUIRED) # Keep this in case other info is useful

find_library(LIBSSH_LIBRARY NAMES ssh PATHS /usr/lib/x86_64-linux-gnu /usr/lib /usr/local/lib REQUIRED)
message(STATUS "Found LIBSSH_LIBRARY: ${LIBSSH_LIBRARY}")

pybind11_add_module(chronicle
    src/core/chronicle.cpp
    src/core/error_handler.cpp
    src/core/ssh.cpp
    src/core/config.cpp
    src/core/device_factory.cpp
    src/bindings/chronicle.cpp
    3party/inih/ini.c
    3party/inih/cpp/INIReader.cpp
)

set_source_files_properties(
    3party/inih/ini.c
    PROPERTIES
    LANGUAGE C
)

# Link directly against the found library
target_link_libraries(chronicle PRIVATE ${LIBSSH_LIBRARY})

set_target_properties(chronicle
    PROPERTIES
    OUTPUT_NAME "chronicle"
    SUFFIX ".so"
)

install(TARGETS chronicle DESTINATION .)