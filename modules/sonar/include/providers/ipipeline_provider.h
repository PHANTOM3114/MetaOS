#pragma once

//Standard Includes
#include <string>

class IPipelineProvider {
public:
    virtual ~IPipelineProvider() = default;
    virtual std::string FetchStatusAsJson() const = 0;

    virtual void SetToken(const std::string& token) = 0;
    virtual std::string GetProviderName() const = 0;
};