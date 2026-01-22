# CI/CD Module

A system service for monitoring your development pipelines directly from the OS.

## 🏛️ Architecture

The CI/CD module is designed as an independent service that integrates with the MetaOS ecosystem via D-Bus, adhering to modern Linux standards for robust Inter-Process Communication. Its primary function is to fetch the status of CI/CD pipelines and provide this data to other system components, such as **Metahub**.

> **Note:** Currently, the service is designed to work only with **GitHub Actions**. Support for other CI/CD platforms like GitLab CI or Jenkins is planned for future releases.

The module consists of two main logical components:

1.  **`ActionsDataFetcher`**: This component is responsible for communicating with the GitHub API. It reads the necessary authentication token (GITHUB_TOKEN) from a `.env` file and sends an authorized GET request to the GitHub Actions endpoint to retrieve the status of recent workflow runs. The fetched data is returned as a JSON string.

2.  **`DataSender`**: This component acts as a D-Bus server. It exposes a D-Bus interface (`org.metaos.CiCd.Interface`) with a method `PipelineStatusFetch`. When this method is called by a D-Bus client (e.g., Metahub), it triggers the `ActionsDataFetcher` to get the latest pipeline status and returns it over D-Bus.

This service-oriented design allows for a clean separation of concerns: the module handles the complexity of API interaction, while other applications can easily consume the data through a standardized D-Bus interface without needing to know the implementation details.

## 🚀 Build and Run

### For macOS & Linux

#### Build

To build the module, run the following command from the root of the repository:

```bash
# Using the Docker wrapper script (recommended for macOS)
./bazel.sh build //components/cicd_module:cicd_module

# For native Linux
bazel build //components/cicd_module:cicd_module
```
### For Windows
Note: All detailed instructions for building and running on Windows will be added in the future. 

## IDE Integration (Autocomplete)

To enable features like autocomplete and go-to-definition in your IDE (e.g., VSCode with clangd), you need to generate a compile_commands.json file. This file tells the IDE how to correctly interpret your C++ code, including all necessary paths and compiler flags.

We have a pre-configured target to generate this file for all main components.

## Generating compile_commands.json

Run the following command from the root of the repository:

### For macOS (recommended)
```bash
./bazel.sh run //:refresh_compile_commands
```

### For native Linux
```bash
bazel run //:refresh_compile_commands
```
> This will create a compile_commands.json file in your project root. Your IDE should automatically detect it and enable advanced code intelligence features.

## 🗺️ Roadmap

The following features are planned for the future development of this module. No specific deadlines are set, but they represent the intended direction of the project.

* **OAuth Authentication:** Transition from the current personal access token model to a more secure and flexible OAuth2 flow for GitHub API authentication
* **GitLab CI Integration:** Add support for fetching pipeline statuses from GitLab instances
* **Jenkins Integration:** Implement a connector to monitor jobs and build statuses from Jenkins servers