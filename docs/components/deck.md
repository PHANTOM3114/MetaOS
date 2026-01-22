# Metahub

The central graphical command center for the MetaOS ecosystem, built using Qt/QML. Metahub serves as the primary **local interface** for users to manage, monitor, and interact with various system components and modules, embodying the "local cycle" within the MetaOS architecture.

## 🏛️ Architecture

> **Note:** This is an initial architectural draft. The final implementation, code, and design may change significantly as development and refactoring progress.

Metahub is designed with a focus on modularity and inter-process communication via D-Bus to deliver a flexible and visually coherent user experience.

Key architectural aspects:

1.  **Modular Interface:** The UI is intended to be modular, allowing different system components (like the `cicd_module`, `vpn_module`, `matrix_module`, etc.) to register their own management panels within Metahub.
2.  **D-Bus Communication:** Metahub relies heavily on the system's D-Bus interface for interacting with other MetaOS modules. It primarily functions as a **D-Bus client** to request data or trigger actions in services like `cicd_module`. Additionally, Metahub exposes its own **D-Bus interface**, allowing other modules to query its state or receive signals crucial for **coordinating actions** across the ecosystem.

This architecture allows Metahub to be the user-facing "front-end" while keeping the core logic within dedicated backend services and modules, promoting separation of concerns.

## 🚀 Build and Run

### For macOS & Linux

#### Build

To build the module, run the following command from the root of the repository:

```bash
# Using the Docker wrapper script (recommended for macOS)
# Note: This currently uses a CMake bridge due to Bazel/Qt integration challenges.
./bazel.sh build //components/metahub:metahub

# For native Linux
# Note: This currently uses a CMake bridge due to Bazel/Qt integration challenges.
bazel build //components/metahub:metahub