#pragma once

//Standard Includes
#include <fstream>
#include <string>
#include <unordered_map>

//Libs
#include "httplib.h"
#include "nlohmann/json.hpp"

class ActionsDataFetcher {
public:
    ActionsDataFetcher();
    std::string StartFetching();

private:
    std::string github_token; // make with vector in the future for storing multiple repositories
    std::string pipeline_info;
};