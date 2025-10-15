#include "adapters/cicd_dbus_adapter.h"
#include "providers/github_provider.h" 
#include <nlohmann/json.hpp> 

CiCdDbusAdapter::CiCdDbusAdapter(sdbus::IConnection& connection, sdbus::ObjectPath objectPath)
    : AdaptorInterfaces(connection, std::move(objectPath))
{
    std::cout << "CiCdDbusAdapter constructor" << std::endl;
    providers_.push_back(std::make_unique<GitHubProvider>());
    
    registerAdaptor();
}

CiCdDbusAdapter::~CiCdDbusAdapter()
{
    std::cout << "CiCdDbusAdapter destructor" << std::endl;
    unregisterAdaptor();
}

std::string CiCdDbusAdapter::PipelineStatusFetch()
{
    std::cout << "PipelineStatusFetch called" << std::endl;

    nlohmann::json results_array = nlohmann::json::array();

    for (const auto& provider : providers_)
    {
        std::string provider_json_string = provider->FetchStatusAsJson();
        if (!provider_json_string.empty())
        {
            results_array.push_back(nlohmann::json::parse(provider_json_string));
        }
    }

    return results_array.dump(4);
}