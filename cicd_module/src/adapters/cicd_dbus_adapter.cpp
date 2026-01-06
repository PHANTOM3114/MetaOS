#include "adapters/cicd_dbus_adapter.h"
#include <nlohmann/json.hpp>
#include "providers/github_provider.h"

CiCdDbusAdapter::CiCdDbusAdapter(sdbus::IConnection& connection, sdbus::ObjectPath objectPath)
    : AdaptorInterfaces(connection, std::move(objectPath)) {
    std::cout << "CiCdDbusAdapter constructor" << std::endl;
    providers_.push_back(std::make_unique<GitHubProvider>());

    registerAdaptor();
}

CiCdDbusAdapter::~CiCdDbusAdapter() {
    std::cout << "CiCdDbusAdapter destructor" << std::endl;
    unregisterAdaptor();
}

std::string CiCdDbusAdapter::PipelineStatusFetch() {
    using namespace std::chrono;
    std::cout << "PipelineStatusFetch called" << std::endl;

    nlohmann::json results_array = nlohmann::json::array();

    for (const auto& provider : providers_) {

        std::cout << provider << std::endl;

        auto cpu_start = high_resolution_clock::now();

        std::string provider_json_string = provider->FetchStatusAsJson();

        auto cpu_end = high_resolution_clock::now();
        auto cpu_duration_us = duration_cast<microseconds>(cpu_end - cpu_start).count();
        std::cout << "[TIMING] CPU-bound (adapter dispatch + polymorphism): " << cpu_duration_us << " us" << std::endl;

        if (!provider_json_string.empty()) {
            results_array.push_back(nlohmann::json::parse(provider_json_string));
        }
    }

    return results_array.dump(4);
}