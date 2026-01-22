//Libs
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/IObject.h>
#include <sdbus-c++/Types.h>

#include <adapters/dbus_adapter.h>

int main() {

    sdbus::ServiceName serviceName{"org.ars.sonar"};
    auto connection = sdbus::createSessionBusConnection(serviceName);

    sdbus::ObjectPath objectPath{"/org/ars/sonar"};
    DbusAdapter adapter(*connection, std::move(objectPath));

    connection->enterEventLoop();

    return 0;
}
