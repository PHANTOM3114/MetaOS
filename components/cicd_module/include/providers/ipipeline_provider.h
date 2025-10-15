#pragma once
#include <string>

class IPipelineProvider {
public:
    virtual ~IPipelineProvider() = default;
    virtual std::string FetchStatusAsJson() const = 0;
};