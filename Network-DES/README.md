# Network Discrete Event Simulator (Network-DES)

A comprehensive C++ network simulation framework implementing discrete event simulation for network performance analysis and research.

## Overview

Network-DES is a discrete event simulation framework designed for network performance analysis. It provides a flexible architecture for simulating network topologies, protocols, and traffic patterns with detailed event-driven modeling.

## Features

- **Discrete Event Simulation Engine**: Core simulation framework with event scheduling and processing
- **Network Nodes**: Support for different node types including hosts and routers
- **Link Modeling**: Configurable network links with bandwidth and latency properties
- **Packet Processing**: Comprehensive packet handling with headers and payload management
- **Protocol Support**: Extensible protocol framework for network layer implementations
- **Performance Tracking**: Built-in packet time tracking and performance metrics
- **Logging System**: Comprehensive logging for debugging and analysis

## Architecture

The simulator follows a modular architecture with the following core components:

### Core Components
- **Simulator**: Main simulation engine managing event scheduling and execution
- **Event System**: Event-driven architecture for network operations
- **Node Framework**: Base classes for network nodes (hosts, routers)
- **Link Management**: Network link abstraction with configurable properties
- **Packet System**: Packet creation, processing, and tracking

### Key Classes
- `Simulator`: Main simulation controller
- `Event`: Base event class for discrete event simulation
- `Node`: Base class for network nodes
- `Host`: End-host implementation
- `Router`: Network router implementation
- `Link`: Network link with bandwidth/latency modeling
- `Packet`: Network packet with headers and payload
- `Interface`: Network interface abstraction

## Building the Project

### Prerequisites
- C++17 compatible compiler (GCC/Clang)
- CMake 3.10 or higher
- Make build system

### Build Instructions

1. Create a build directory and configure the project:
    ```sh
    mkdir build
    cd build
    cmake ..
    ```

2. Build the project:
    ```sh
    make
    ```

3.  Run File
    ```sh
    ./NetworkSimulator > output
    ```

    Logs will be in output whereas metainfo will be printed on the screen.
