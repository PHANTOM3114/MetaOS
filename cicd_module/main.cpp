//Libs
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/IObject.h>
#include <sdbus-c++/Types.h>

#include <adapters/cicd_dbus_adapter.h>

int main() {

    sdbus::ServiceName serviceName{"org.metaos.CiCdService"};
    auto connection = sdbus::createSessionBusConnection(serviceName);

    sdbus::ObjectPath objectPath{"/org/metaos/CiCd"};
    CiCdDbusAdapter adapter(*connection, std::move(objectPath));

    connection->enterEventLoop();

    return 0;
}
