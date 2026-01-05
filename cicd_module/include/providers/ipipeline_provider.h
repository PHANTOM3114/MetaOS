#pragma once

//Standard Includes
#include <string>

class IPipelineProvider {
public:
    virtual ~IPipelineProvider() = default;
    virtual std::string FetchStatusAsJson() const = 0;
};