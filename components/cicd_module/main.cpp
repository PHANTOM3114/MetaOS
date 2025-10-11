//Standart Includes
#include <iostream>
#include <string>
#include <cstdlib>

//Libs
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/IObject.h>
#include <sdbus-c++/Types.h>

#include "fetcher.h"
#include "sender.h"

int main() {
    try {
        auto current_path = std::filesystem::current_path();
        std::cout << "!!! DEBUG: Current working directory is: " << current_path << std::endl;

        std::string filename = ".env";
        std::ifstream env_file(filename);

        if (env_file.is_open()) {
            std::cout << "!!! DEBUG: SUCCESS! File '.env' opened. Contents:" << std::endl;
            std::string line;
            while (std::getline(env_file, line)) {
                std::cout << "!!! DEBUG: " << line << std::endl;
            }
            env_file.close();
        } else {
            std::cerr << "!!! DEBUG: FAILED to open '.env' file from the current directory." << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "!!! DEBUG: Filesystem error: " << e.what() << std::endl;
    }
    sdbus::ServiceName serviceName{"org.metaos.CiCdService"};
    auto connection = sdbus::createBusConnection(serviceName);

    sdbus::ObjectPath objectPath{"/org/metaos/CiCd"};
    DataSender sender(*connection, std::move(objectPath));

    connection->enterEventLoop();

    return 0;
}
