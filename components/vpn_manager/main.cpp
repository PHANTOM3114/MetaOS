#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/Message.h>
#include <sdbus-c++/Types.h>
#include <sdbus-c++/sdbus-c++.h>
#include <iostream>
#include <vector>

int main() {

    auto connection = sdbus::createSystemBusConnection();

    sdbus::ServiceName destination{"org.freedesktop.NetworkManager"};
    sdbus::ObjectPath objectPath{"/org/freedesktop/NetworkManager/Settings"};

    auto networkManagerProxy = sdbus::createProxy(std::move(connection), std::move(destination), std::move(objectPath));

    sdbus::InterfaceName interfaceName{"org.freedesktop.NetworkManager.Settings"};

    sdbus::MethodName getListConnections{"ListConnections"};
    auto method = networkManagerProxy->createMethodCall(interfaceName, getListConnections);
    auto reply = networkManagerProxy->callMethod(method);

    std::vector<sdbus::ObjectPath> result;
    reply >> result;

    for (auto elements : result) {
        std::cout << elements << std::endl;
    }

    return 0;
}