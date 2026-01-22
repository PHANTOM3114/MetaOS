#pragma once
//Standard Includes
#include <fstream>
#include <string>
#include <unordered_map>

//Libs
#include "httplib.h"
#include "nlohmann/json.hpp"

#include "ipipeline_provider.h"

class GitHubProvider : public IPipelineProvider {
public:
    GitHubProvider();
    std::string FetchStatusAsJson() const override;

    void SetToken(const std::string& token) override;
    std::string GetProviderName() const override;

private:
    std::string current_token_;
    std::string pipeline_info;
};