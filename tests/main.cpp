#include "core/ssh.hpp"
#include "core/config.hpp"
#include "core/error_handler.hpp"
#include <vector>

using namespace std;

int main() {
    readChronicleConfig(CHRONICLE_CONFIG);
    std::vector<std::string> dir_items = getDirContents("alum-server", "/home/", &chronicleConfig);
    for (auto i : dir_items) {
        std::cout << "Item: " << i << std::endl;
    }
    return 0;
}
