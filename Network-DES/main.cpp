#include "include/event.hpp"
#include "include/interface.hpp"
#include "include/link.hpp"
#include "include/logger.hpp"
#include "include/nodes/host.hpp"
#include "include/nodes/router.hpp"
#include "include/packets/packet-time-tracker.hpp"
#include "include/simulator.hpp"

#include <memory>

int main() {

    NetSim::Logger::setLevel(NetSim::Logger::Level::DEBUG);

    NetSim::Logger::log(NetSim::Logger::Level::INFO, 0.0, NetSim::Logger::SIMULATOR, "SIM",
                        "Starting Network Discrete Event Simulator Demo");

    // Create simulator
    Simulator simulator;
    NetSim::Logger::log(NetSim::Logger::Level::DEBUG, 0.0, NetSim::Logger::SIMULATOR, "SIM",
                        "Simulator instance created");

    NetSim::Logger::log(NetSim::Logger::Level::INFO, 0.0, NetSim::Logger::GENERAL, "SETUP",
                        "Initializing network topology: HostA <-> Router1 <-> HostB");


    // Create nodes: Two hosts and one router
    auto hostA = std::make_shared<Host>("HostA");
    auto hostB = std::make_shared<Host>("HostB");
    auto router1 = std::make_shared<Router>("R1");

    NetSim::Logger::log(NetSim::Logger::Level::DEBUG, 0.0, NetSim::Logger::HOST, "HostA", "Host created");
    NetSim::Logger::log(NetSim::Logger::Level::DEBUG, 0.0, NetSim::Logger::HOST, "HostB", "Host created");
    NetSim::Logger::log(NetSim::Logger::Level::DEBUG, 0.0, NetSim::Logger::ROUTER, "R1", "Router created");

    // Create links
    auto linkAR = std::make_shared<Link>("Link_A_R1", 100.0, 5.0); // HostA to Router1: 100 Mbps, 5ms delay
    auto linkRB = std::make_shared<Link>("Link_R1_B", 100.0, 5.0); // Router1 to HostB: 100 Mbps, 5ms delay

    // NetSim::Logger::log(NetSim::Logger::Level::INFO, 0.0, NetSim::Logger::LINK, "LINKS",
    //                     "Created links: 100 Mbps bandwidth, 5ms propagation delay");

    // Create interfaces
    auto interfaceA = std::make_shared<Interface>("IF_A", hostA, linkAR);
    auto interfaceR1 = std::make_shared<Interface>("IF_R1_A", router1, linkAR);
    auto interfaceR2 = std::make_shared<Interface>("IF_R1_B", router1, linkRB);
    auto interfaceB = std::make_shared<Interface>("IF_B", hostB, linkRB);

    NetSim::Logger::log(NetSim::Logger::Level::DEBUG, 0.0, NetSim::Logger::INTERFACE, "IF",
                        "Network interfaces created and configured");

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
    // NetSim::Logger::log(NetSim::Logger::Level::INFO, 0.0, NetSim::Logger::ROUTER, "R1",
    //                     "Routing table configured: HostA <-> IF_R1_A, HostB <-> IF_R1_B");

    // Create packet time tracker
    auto tracker = std::make_unique<PacketTimeTracker>(&simulator);

    // Set initial time for the packet (1.0 seconds)
    tracker->currentTime = 1.0;
    // NetSim::Logger::log(NetSim::Logger::Level::DEBUG, 1.0, NetSim::Logger::PACKET, "TRACKER",
    //                     "Packet time tracker initialized");

    // Generate a packet from HostA to HostB
    // NetSim::Logger::log(NetSim::Logger::Level::INFO, 1.0, NetSim::Logger::GENERAL, "DEMO",
    //                     "Starting packet transmission demo");

    hostA->generatePacket("HostB", "Hello from HostA to HostB!", tracker.get());

    // Note: Keep tracker alive for the simulation duration
    // In a real implementation, you'd want better lifetime management
    auto *trackerPtr = tracker.release(); // Transfer ownership manually for now

    // Run simulation
    simulator.run();

    NetSim::Logger::log(NetSim::Logger::Level::INFO, simulator.getCurrentTime(), NetSim::Logger::GENERAL, "DEMO",
                        "Demo completed successfully!");

    // Clean up manually managed tracker
    delete trackerPtr;

    return 0;
}
