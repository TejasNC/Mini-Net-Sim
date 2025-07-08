#include "include/event.hpp"
#include "include/interface.hpp"
#include "include/link.hpp"
#include "include/nodes/host.hpp"
#include "include/nodes/router.hpp"
#include "include/packets/packet-time-tracker.hpp"
#include "include/simulator.hpp"

#include <iostream>
#include <memory>

int main() {
    std::cout << "Starting Network Discrete Event Simulator Demo..." << std::endl;

    // Create simulator
    Simulator simulator;

    // Create nodes: Two hosts and one router
    auto hostA = std::make_shared<Host>("HostA");
    auto hostB = std::make_shared<Host>("HostB");
    auto router1 = std::make_shared<Router>("R1");

    // Create links
    auto linkAR = std::make_shared<Link>("Link_A_R1", 100.0, 5.0); // HostA to Router1: 100 Mbps, 5ms delay
    auto linkRB = std::make_shared<Link>("Link_R1_B", 100.0, 5.0); // Router1 to HostB: 100 Mbps, 5ms delay

    // Create interfaces
    auto interfaceA = std::make_shared<Interface>("IF_A", hostA, linkAR);
    auto interfaceR1 = std::make_shared<Interface>("IF_R1_A", router1, linkAR);
    auto interfaceR2 = std::make_shared<Interface>("IF_R1_B", router1, linkRB);
    auto interfaceB = std::make_shared<Interface>("IF_B", hostB, linkRB);

    // Connect interfaces to links
    linkAR->connectInterface(interfaceA);
    linkAR->connectInterface(interfaceR1);
    linkRB->connectInterface(interfaceR2);
    linkRB->connectInterface(interfaceB);

    // Add interfaces to nodes
    hostA->addInterface(interfaceA);
    router1->addInterface(interfaceR1);
    router1->addInterface(interfaceR2);
    hostB->addInterface(interfaceB);

    // Set up routing table for router
    router1->addRoute("HostA", interfaceR1); // Route to HostA via interfaceR1
    router1->addRoute("HostB", interfaceR2); // Route to HostB via interfaceR2

    // Create packet time tracker
    auto tracker = std::make_unique<PacketTimeTracker>(&simulator);

    // Set initial time for the packet (1.0 seconds)
    tracker->currentTime = 1.0;

    // Generate a packet from HostA to HostB
    std::cout << "\n=== Setting up packet transmission from HostA to HostB ===" << std::endl;
    hostA->generatePacket("HostB", "Hello from HostA to HostB!", tracker.get());

    // Note: Keep tracker alive for the simulation duration
    // In a real implementation, you'd want better lifetime management
    auto *trackerPtr = tracker.release(); // Transfer ownership manually for now

    // Run simulation
    std::cout << "\n=== Running Simulation ===" << std::endl;
    simulator.run();

    std::cout << "\nSimulation completed!" << std::endl;

    // Clean up manually managed tracker
    delete trackerPtr;

    return 0;
}
