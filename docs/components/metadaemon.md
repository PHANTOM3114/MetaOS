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

## Roadmap

- **Q3 2025:** Completed the `Shell Controller` MVP (basic command execution via `forkpty` and `epoll`).
- **Q4 2025:** Develop the `CI/CD Controller` MVP and a dashboard module for GitHub Actions.

---
