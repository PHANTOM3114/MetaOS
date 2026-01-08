#include "providers/github_provider.h"
#include <absl/base/log_severity.h>

//Libs
#include <libenvpp/env.hpp>
#include <string>

GitHubProvider::GitHubProvider() {
    std::ifstream env_file(".env");
    if (!env_file.is_open()) {
        throw std::runtime_error("FATAL: Cannot open .env file. Check path and permissions.");
    }

    std::string line;
    bool token_found = false;

    while (std::getline(env_file, line)) {
        std::string key = "GITHUB_TOKEN=";
        if (line.rfind(key, 0) == 0) {
            github_token_ = line.substr(key.length());
            token_found = true;
            break;
        }
    }
    env_file.close();

    if (!token_found || github_token_.empty()) {
        throw std::runtime_error("Error: 'GITHUB_TOKEN' key not found or is empty in .env file.");
    }
}

std::string GitHubProvider::FetchStatusAsJson() const {

    std::string pipeline_info = "";

    try {
        std::string auth_header = "token " + github_token_;

        httplib::Client cli("https://api.github.com");
        cli.set_connection_timeout(30, 0);  // 30 seconds
        cli.set_read_timeout(60, 0);        // 60 seconds

        httplib::Headers headers = {
            {"User-Agent", "MetaOS-CiCD-Module-Test/1.0"}, {"Accept", "application/vnd.github.v3+json"}, {"Authorization", auth_header}};

        auto result = cli.Get("/repos/PHANTOM3114/MetaOS/actions/runs", headers);

        if (result && result->status == 200) {
            std::cout << "Status Code: " << result->status << std::endl;
            if (!result->body.empty()) {
                try {
                    nlohmann::json raw_json = nlohmann::json::parse(result->body);
                    nlohmann::json filtered_json = nlohmann::json::array();

                    if (raw_json.contains("workflow_runs")) {
                        for (auto& el : raw_json["workflow_runs"]) {
                            nlohmann::json item;
                            item["id"] = el["id"];
                            item["name"] = el["name"];
                            item["status"] = el["status"];
                            item["conclusion"] = el["conclusion"];

                            filtered_json.push_back(item);
                        }
                    }

                    std::ofstream jsonfile;
                    jsonfile.open("file");
                    jsonfile << filtered_json;
                    jsonfile.close();

                    pipeline_info = filtered_json.dump(4);

                    return pipeline_info;
                } catch (const nlohmann::json::exception& e) {
                    std::cerr << "JSON parsing error: " << e.what() << std::endl;
                    return "";
                }
            }
            return pipeline_info;
        } else if (result) {
            std::cerr << "HTTP Error: Status " << result->status << std::endl;
            if (!result->body.empty()) {
                std::cerr << "Response: " << result->body << std::endl;
            }
            return "";
        } else {
            auto err = result.error();
            std::cerr << "HTTP request failed: " << httplib::to_string(err) << std::endl;
            return "";
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in StartFetching: " << e.what() << std::endl;
        return "";
    }
}
