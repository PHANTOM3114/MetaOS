# MetaDaemon

GRPC-based Main Network Daemon of MetaOS Ecosystem, which is point of remote management of all other daemons.

## Architecture

The system is built on the interaction of two key components:

* **Controllers:** Logical units within `MetaDaemon` responsible for a specific functional area which will communicate with modules.
* **Modules:** Separate, independent applications that perform this functions. 

## Core Controllers & Modules

| Component | Status | Description |
|---|---|---|
|**Shell Controller** | ![Status](https://img.shields.io/badge/status-completed-green)|Provides secure access to the system shell and command execution.
|**CI/CD Controller**| ![Status](https://img.shields.io/badge/status-planned-lightgrey) | Enables monitoring and management of pipelines
|**Cloud Controller**| ![Status](https://img.shields.io/badge/status-planned-lightgrey) | Integration with cloud services and providers for remote access.
| **VPN Controller** | ![Status](https://img.shields.io/badge/status-planned-lightgrey) | Manages VPN connections based on predefined scenarios
|**AI Controller** | ![Status](https://img.shields.io/badge/status-planned-lightgrey)|Provides secure access to the system shell and command execution.

## 🚀 Build and Run

### For macOS & Linux

#### Build

To build the module, run the following command from the root of the repository:

```bash
# Using the Docker wrapper script (recommended for macOS)
./bazel.sh build //components/metadaemon:server

# For native Linux
bazel build //components/metadaemon:server
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
