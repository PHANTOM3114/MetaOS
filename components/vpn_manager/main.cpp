#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/Message.h>
#include <sdbus-c++/Types.h>
#include <sdbus-c++/sdbus-c++.h>
#include <iostream>
#include <vector>

int main() {

    auto connection = sdbus::createSystemBusConnection();

    sdbus::ServiceName networkManagerDestination{"org.freedesktop.NetworkManager"};
    sdbus::ObjectPath networkManagerObjectPath{"/org/freedesktop/NetworkManager"};

    auto networkManagerProxy = sdbus::createProxy(*connection, networkManagerDestination, std::move(networkManagerObjectPath));

    sdbus::InterfaceName interfaceName{"org.freedesktop.NetworkManager"};

    auto activeConnections = networkManagerProxy->getProperty("ActiveConnections").onInterface(interfaceName);
    auto activeConnectionsList = activeConnections.get<std::vector<sdbus::ObjectPath>>();

    if (!activeConnectionsList.empty()) {
        sdbus::ObjectPath activeConnectionObjectPath{activeConnectionsList.at(0)};
        auto currentConnectionProxy = sdbus::createProxy(*connection, networkManagerDestination, std::move(activeConnectionObjectPath));

        auto currentConnectionInfo = currentConnectionProxy->getProperty("Vpn").onInterface("org.freedesktop.NetworkManager.Connection.Active");
        auto state = currentConnectionInfo.get<bool>();

        std::cout << state << std::endl;
    }

    return 0;
}