# Matrix Module

A central communication hub for the MetaOS ecosystem, built on the decentralized [Matrix](https://matrix.org/) protocol. This module serves two primary functions: it aggregates all external user communications into one sovereign space, and simultaneously acts as a secure, privileged assistant for handling critical system-level data and events.

## 🏛️ Architecture

> **Note:** This is an initial architectural draft. The final implementation, code, and design may change significantly as development and refactoring progress.

The module is built on the decentralized, end-to-end encrypted [Matrix](https://matrix.org/) protocol. Architecturally, it is divided into 3 independent microservices (separate processes) to adhere to the Single Responsibility Principle (SRP) and ensure maximum robustness.

1.  **`Communication Bridge`**
    This service is responsible for all external-facing interactions. Its primary function is to act as a universal parser and bridge to connect with other proprietary messengers (such as Telegram, WhatsApp, Signal, etc.). This allows the user to aggregate all their disparate chats into one secure, sovereign "Matrix Space" managed by MetaOS.

2.  **`Critical Information Administrator`**
    This service is system-facing ("looking inward"). It functions as a privileged local daemon responsible for **managing situations and data classified as sensitive.** While Metahub handles routine management, the Matrix module handles exceptions and high-stakes events. Its responsibilities include:
    * **Data Classification & Handling:** Intercepting alerts from other modules (e.g., `vpn_module`), classifying them by sensitivity, and securely routing them to a dedicated, private Matrix room.
    * **Situational Response:** Providing the user with the tools to manage these critical incidents by executing pre-defined protocols or direct commands *in response* to an alert. This creates a secure, event-driven channel for system administration that is isolated from less secure components.
3.  **`Secure Transport Layer`**
    This service utilizes Matrix as a secure, encrypted transport bus for device-to-device synchronization, effectively replacing local network discovery protocols with a globally accessible, encrypted channel.
    * **Universal Clipboard:** Operates via a **persistent background daemon** deployed by the mobile client. Unlike standard app-based solutions, this daemon independently monitors the system clipboard and securely transmits data over the Matrix protocol. This ensures seamless synchronization in the background, regardless of the main application's lifecycle state.
    * **Secure File Exchange:** A native, **"AirDrop-like" sharing experience** integrated directly into the MetaOS and Android. It allows for instant, encrypted file transfer between trusted devices without launching specific applications. The user experience is seamless: select a file, tap the system share target, and the transfer occurs securely over the Matrix protocol.
    
This separation ensures that a potential failure in an external-facing bridge (e.g., due to an API change by Telegram) will not affect the critical operation of the `System Administrator` module.

## 🚀 Build and Run

### For macOS & Linux

#### Build

To build the module, run the following command from the root of the repository:

```bash
# Using the Docker wrapper script (recommended for macOS)
./bazel.sh build //components/matrix:matrix

# For native Linux
bazel build //components/matrix:matrix