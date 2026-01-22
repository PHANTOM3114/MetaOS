# AR-S
> **Developed by RPC AR-X**

> *"You cannot trust a system you cannot break."*

**AR-S (Advanced Reliable System)** is a sovereign operating system built on top of the Linux kernel. It is grounded on a simple premise: your computer should be a powerful, integrated tool that serves you, not the other way around. We combine the flexibility of Linux with a deep, thoughtful integration of services to create an environment that is secure, controllable, and inspiring.

Instead of hiding powerful tools, we provide a unified interface to control them. Instead of isolated applications, we build a cohesive system. AR-S is designed to be your digital command center and your creative workshop.

**Table of contents**

1.  [Philosophy](#philosophy)
2.  [Core Architecture](#core-architecture)
3.  [System Components](#system-components)
4.  [Getting Started](#getting-started)
5.  [Roadmap](#roadmap)

---
## Philosophy

The development and philosophy of AR-S are structured around three distinct principles, embodied by their respective sectors:

* **Security Sector - Privacy by Default**: We believe privacy is a non-negotiable right. This is achieved by integrating natively secure components like the Matrix protocol and building autonomous, scenario-based security tools. Your data and communications should be yours alone.

* **Control Sector - Extensibility by Design**: Your system should adapt to you, not the other way around. Adding new features — from local services to remote controllers — is designed to be simple and not compromise system stability. You are the architect of your digital environment.

* **Utility Sector - Compatibility without Compromise**: We address the challenge of using software not natively ported to Linux. This principle is achieved by simplifying the use of existing tools to ensure you can work and play without limitations.

---
## Core Architecture

AR-S employs a modular, service-oriented architecture. The ecosystem comprises distinct, autonomous components that interact through standard IPC protocols: **gRPC** (for remote operations) and **D-Bus** (for local system calls).

The architecture relies on three key layers:

* **Modules:** Independent executables responsible for specific functional domains (e.g., `sonar`, `magfield`). They operate autonomously and expose their status via D-Bus.
* **Agent:** The **remote orchestration daemon**. It runs in the background, managing the lifecycle of modules and exposing a gRPC interface for external control and automation.
* **Deck:** The **local management interface**. A unified Qt-based desktop application that provides direct visual control over the system modules and configuration.

This design decouples the backend logic from the frontend, ensuring system stability and seamless extensibility.

---
## System Components

### Security Sector

| Component | Status | Description |
|---|---|---|
| **Magfield** | ![Status](https://img.shields.io/badge/status-planned-lightgrey) | System-level integration with scenario-based automation. |
| **Interlink** | ![Status](https://img.shields.io/badge/status-in%20progress-yellow) | Native integration for the decentralized, encrypted Matrix protocol. |
| **Face ID** | ![Status](https://img.shields.io/badge/status-planned-lightgrey) | Secure login using webcam-based facial recognition. |
| **Security Hub**| ![Status](https://img.shields.io/badge/status-planned-lightgrey) | Integration with smart security systems (Ajax, Bosch, etc.). |

### Control Sector

| Component | Status | Description |
|---|---|---|
|**Deck** | ![Status](https://img.shields.io/badge/status-active-green)|The central Qt-based desktop application for managing and interacting with all system components. |
|**Agent**| ![Status](https://img.shields.io/badge/status-active-green) | Point of remote management for all system daemons. |
|**Android Client**| ![Status](https://img.shields.io/badge/status-in%20progress-yellow) | A native mobile client for remote management, notifications, and deep integration. |

### Utility Sector

| Component | Status | Description |
|---|---|---|
|**Sonar** | ![Status](https://img.shields.io/badge/status-active-green)| A dedicated module for monitoring development pipelines (CI/CD) directly from the OS. |
|**D-Bus Integration** | ![Status](https://img.shields.io/badge/status-core-blue)| Adherence to modern Linux standards for robust and efficient Inter-Process Communication. |
|**GPU Passthrough** | ![Status](https://img.shields.io/badge/status-planned-lightgrey) | Simplified configuration for virtualization and gaming performance. |

---
## Getting Started

**Architectural Requirement:** AR-S is designed and built exclusively for the **Linux** operating system. Therefore, all development must take place within a Linux environment to ensure full compatibility.

### Environment Setup

The project's development environment is currently undergoing refinement to ensure a stable and straightforward setup process. To avoid providing potentially outdated instructions, we ask that you refer to the official documentation for setting up the core dependencies.

1. **Install System Dependencies:** Debian/Ubuntu:
    
    sudo apt update && sudo apt install -y build-essential cmake git clang lld pkg-config libssl-dev zlib1g-dev libsqlite3-dev nlohmann-json3-dev qt6-base-dev qt6-declarative-dev qt6-tools-dev libsdbus-c++-dev libgrpc++-dev libprotobuf-dev libnl-3-dev libnl-genl-3-dev protobuf-compiler-grpc
  
If you are on the other distro, package names may be different

2.  **For non-Linux platforms (macOS / Windows):** The officially supported method is to use an isolated development container.
    
    (Placeholder: A detailed, validated guide for setting up and using the recommended development environment will be added here once the configuration is finalized).

### Building and Running

Build all targets:

    cmake -B build && cmake --build build

> More detailed build and run instructions for each individual component can be found in their respective documentation files within the `/docs` directory.
---
## Roadmap

- [x] **Autumn 2025** - MVP of Deck (UI) with Sonar
- [x] **Winter 2025-2026** - JSON view in Deck & Backend Refactoring
- [ ] **Spring 2026** - MVP of Interlink
