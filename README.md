# MetaOS
**MetaOS** is an distributed service-oriented ecosystem, which grounded on a simple premise: your computer should be a powerful, integrated tool that serves you, not the other way around. We combine the flexibility of Linux with a deep, thoughtful integration of services to create an environment that is secure, controllable, and inspiring.
Instead of hiding powerful tools, we provide a unified interface to control them. Instead of isolated applications, we build a cohesive system. MetaOS is designed to be your digital command center and your creative workshop.

**Table of contents**

1.  [Philosophy](#-philosophy)
2.  [Core Architecture](#️-core-architecture)
3.  [System Components](#️-system-components)
4.  [Getting Started](#-getting-started)
5.  [Roadmap](#️-roadmap)

---
## 🧠 Philosophy

The development and philosophy of MetaOS are structured around three distinct principles, embodied by their respective sectors:

* 🛡️ **Security Sector - Privacy by Default**: We believe privacy is a non-negotiable right. This is achieved by integrating natively secure components like the Matrix protocol and building autonomous, scenario-based security tools like the built-in VPN. Your data and communications should be yours alone.

* 📲 **Control Sector - Extensibility by Design**: Your system should adapt to you, not the other way around. Adding new features — from local services to remote controllers — is designed to be simple and not compromise system stability. You are the architect of your digital environment.

* ⚙️ **Utility Sector - Compatibility without Compromise**: We address the challenge of using software not natively ported to Linux. This principle is achieved by simplifying the use of existing tools, such as GPU Passthrough, to ensure you can work and play without limitations.

---
## 🏗️ Core Architecture

MetaOS is not a monolithic application but an ecosystem of interconnected, independent services that communicate via standardized protocols like **gRPC** and **D-Bus**.

The system is built on the interaction of two key component types:

* **Modules:** Separate, independent applications that perform specific functions (e.g., `cicd_module`, `vpn_module`). They are the "organs" of the system.
* **MetaDaemon:** The **"remote cycle"** of the ecosystem. It acts as the central nervous system, with its controllers managing modules through standard D-Bus calls for remote operations.
* **Metahub:** The **"local cycle"** of the ecosystem. This central Qt-based desktop application is your command center for on-site management, providing a unified graphical interface to all system components.

This service-oriented design ensures robustness, flexibility, and makes the entire ecosystem highly extensible.

---
## 🛠️ System Components

### 🛡️ Security Sector ###


| Component | Status | Description |
|---|---|---|
| **Built-in VPN** | ![Status](https://img.shields.io/badge/status-planned-lightgrey) | System-level integration with scenario-based automation. |
| **Matrix Integration** | ![Status](https://img.shields.io/badge/status-planned-lightgrey) | Native integration for the decentralized, encrypted Matrix protocol. |
| **Face ID** | ![Status](https://img.shields.io/badge/status-planned-lightgrey) | Secure login using webcam-based facial recognition. |
| **Security & IoT Hub**| ![Status](https://img.shields.io/badge/status-planned-lightgrey) | Integration with smart security systems like Ajax, Bosch, etc. |

### 📱 Control Sector ###

| Component | Status | Description |
|---|---|---|
|**Metahub** | ![Status](https://img.shields.io/badge/status-in%20progress-yellow)|The central Qt-based desktop application for managing and interacting with all system components. 
|**MetaDaemon**| ![Status](https://img.shields.io/badge/status-in%20progress-yellow) | Point of remote management of all other daemons.
|**Android/iOS Client**| ![Status](https://img.shields.io/badge/status-in%20progress-yellow) | A native mobile client for remote management, notifications, and deep integration with your phone.

### ⚙️ Utility Sector ###

| Component | Status | Description |
|---|---|---|
|**CI/CD Module** | ![Status](https://img.shields.io/badge/status-in%20progress-yellow)| A system service for monitoring your development pipelines directly from the OS. 
|**D-Bus Integration** | ![Status](https://img.shields.io/badge/status-in%20progress-yellow)| Adherence to modern Linux standards for robust and efficient Inter-Process Communication.
|**GPU Passthrough** | ![Status](https://img.shields.io/badge/status-planned-lightgrey) | Simplify your gaming and working experience.

---
## 🚀 Getting Started

**Architectural Requirement:** The MetaOS ecosystem is designed and built exclusively for the **Linux** operating system. Therefore, all development must ultimately take place within a Linux environment to ensure full compatibility.

### Environment Setup

The project's development environment is currently undergoing refinement to ensure a stable and straightforward setup process. To avoid providing potentially outdated instructions, we ask that you refer to the official documentation for setting up the core dependencies.

1.  **Install Bazel:** Please follow the [**official Bazel installation guide**](https://bazel.build/install) for the most up-to-date instructions for your platform. This ensures you are using a compatible and correctly configured version.

2.  **Install System Dependencies:** A complete list of required system libraries (like `g++`, `libsdbus-c++-dev`, etc.) can be found in the project's `Dockerfile`. This file serves as the definitive source for setting up a native Linux environment.

3.  **For non-Linux platforms (macOS / Windows):** The officially supported method is to use an isolated development container.
    **(Placeholder: A detailed, validated guide for setting up and using the recommended development environment will be added here once the configuration is finalized).**

### Building and Running

> Detailed build and run instructions for each individual component can be found in their respective documentation files within the `/docs` directory.
---
## 🗺️ Roadmap

* **Autumn 2025** - MVP of Metahub with CI/CD Module
* **Winter 2025-2026** - MVP of VPN, and yaml parser
