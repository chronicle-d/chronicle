#include <vector>
#include <string>
#include "core/config.hpp"
#include "core/ssh.hpp"

std::vector<std::string> getDirContents(std::string unit_name, std::string dir_path, connectionInfo ci);