//Libs
#include <libenvpp/env.hpp>

#include "fetcher.h"

ActionsDataFetcher::ActionsDataFetcher()
{
    std::ifstream env_file(".env");
    if (!env_file.is_open()) {
        throw std::runtime_error("FATAL: Cannot open .env file. Check path and permissions.");
    }

    std::string line;
    bool token_found = false;

    while (std::getline(env_file, line)) {
        std::string key = "GITHUB_TOKEN=";
        if (line.rfind(key, 0) == 0) {
            github_token = line.substr(key.length());
            token_found = true;
            break;
        }
    }
    env_file.close();

    if (!token_found || github_token.empty()) {
        throw std::runtime_error("Error: 'GITHUB_TOKEN' key not found or is empty in .env file.");
    }
}

std::string ActionsDataFetcher::StartFetching() {
    try {
        std::string auth_header = "token " + github_token;

        httplib::Client cli("https://api.github.com");
        cli.set_connection_timeout(30, 0); // 30 seconds
        cli.set_read_timeout(60, 0);       // 60 seconds

        httplib::Headers headers = {
            { "User-Agent", "MetaOS-CiCD-Module-Test/1.0" },
            { "Accept", "application/vnd.github.v3+json" },
            { "Authorization", auth_header }
        };

        auto res = cli.Get("/repos/PHANTOM3114/MetaOS-Controller/actions/runs", headers);

        if (res && res->status == 200) {
            std::cout << "Status Code: " << res->status << std::endl;
            if (!res->body.empty()) {
                try {
                    nlohmann::json json = nlohmann::json::parse(res->body);
                    pipeline_info = json.dump(4);
                    std::cout << json.dump(4) << std::endl;
                } catch (const nlohmann::json::exception& e) {
                    std::cerr << "JSON parsing error: " << e.what() << std::endl;
                    return "";
                }
            }
            return pipeline_info;
        }
        else if (res) {
            std::cerr << "HTTP Error: Status " << res->status << std::endl;
            if (!res->body.empty()) {
                std::cerr << "Response: " << res->body << std::endl;
            }
            return "";
        }
        else {
            auto err = res.error();
            std::cerr << "HTTP request failed: " << httplib::to_string(err) << std::endl;
            return "";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in StartFetching: " << e.what() << std::endl;
        return "";
    }
}


