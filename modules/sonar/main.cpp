//C++ Standard Includes
#include <chrono>
#include <csignal>
#include <exception>
#include <iostream>
#include <mutex>
#include <thread>

//Libs
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/IObject.h>
#include <sdbus-c++/Types.h>

#include <adapters/dbus_adapter.h>

std::unique_ptr<sdbus::IConnection> g_connection;

void signal_handler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received. Stopping loop...\n";

    if (g_connection) {
        g_connection->leaveEventLoop();
    }
}

int main() {

    try {
        std::signal(SIGINT, signal_handler);
        std::signal(SIGTERM, signal_handler);

        sdbus::ServiceName serviceName{"org.ars.sonar"};
        g_connection = sdbus::createSessionBusConnection(serviceName);

        sdbus::ObjectPath objectPath{"/org/ars/sonar"};
        DbusAdapter adapter(*g_connection, std::move(objectPath));

        g_connection->enterEventLoop();

        std::cout << "Exiting event loop. Sending goodbye signal..." << std::endl;

        adapter.emitSonarStopWorkSignal("Sonar exit gracefully...");
        std::cout << "Signal emitted..." << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

    } catch (std::exception& e) {
        std::cout << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
