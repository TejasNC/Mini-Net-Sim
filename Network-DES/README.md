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
- **Logging System**: Clean, simple logging with component identification and level filtering

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

3. Run the simulator:
    ```sh
    ./NetworkSimulator
    ```

## Current Demo

The included demo simulates a simple three-node topology:
```
HostA <---> Router1 <---> HostB
```

Features demonstrated:
- Packet generation and transmission
- Router forwarding with routing tables
- Realistic transmission and propagation delays
- Event-driven simulation with proper timing
- Detailed logging of all network operations

### Sample Output
```
[0.000] INFO : SIM(SIM)            Starting Network Discrete Event Simulator Demo
[0.000] DEBUG: SIM(SIM)            Simulator instance created
[0.000] INFO :                     Initializing network topology: HostA <-> Router1 <-> HostB
[1.000] INFO : HOST(HostA)         Generating new packet for destination [HostB]
[1.005] INFO : ROUTER(R1)          Forwarding packet to [HostB] via interface [IF_R1_B]
[1.010] INFO : HOST(HostB)         Packet successfully delivered! Processing payload...
[1.010] INFO : SIM(SIM)            Simulation completed successfully! Final time: 1.010004s
```

## Project Status

**Working Features:**
- Basic network topology simulation
- Discrete event simulation engine
- Packet routing through routers
- Transmission delay modeling
- Clean logging system

**Planned Improvements:**
- Automatic routing table generation
- Configuration file support for topologies
- Enhanced topology builder API
- Performance metrics and statistics

## Usage

### Modifying the Simulation

The main simulation is defined in `main.cpp`. To customize:

1. **Change network topology**: Modify node creation and link connections
2. **Adjust link properties**: Set bandwidth and delay in link creation
3. **Configure logging**: Use `NetSim::Logger::setLevel()` to control verbosity
4. **Add more traffic**: Create additional packets with different timing

### Logger Levels
- `DEBUG`: Detailed packet-level information
- `INFO`: Key simulation events and packet flows
- `WARNING`: Unusual conditions
- `ERROR`: Critical failures

Example: `NetSim::Logger::setLevel(NetSim::Logger::Level::INFO);`
