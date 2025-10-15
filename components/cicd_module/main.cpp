//Standart Includes
#include <filesystem>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

//Libs
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/IObject.h>
#include <sdbus-c++/Types.h>

#include <include/adapters/cicd_dbus_adapter.h>

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
    auto connection = sdbus::createSessionBusConnection(serviceName);

    sdbus::ObjectPath objectPath{"/org/metaos/CiCd"};
    CiCdDbusAdapter adapter(*connection, std::move(objectPath));

    connection->enterEventLoop();

    return 0;
}
