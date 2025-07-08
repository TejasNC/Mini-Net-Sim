#include "include/simulator.hpp"
#include "include/router.hpp"
#include "include/interface.hpp"
#include "include/link.hpp"
#include "include/packets/packet.hpp"
#include "include/packets/payload.hpp"
#include "include/event.hpp"

#include <iostream>
#include <memory>

int main() {
    std::cout << "Starting Network Discrete Event Simulator Demo..." << std::endl;

    // Create simulator
    Simulator simulator;

    // Create nodes
    auto router1 = std::make_shared<Router>("R1");
    auto router2 = std::make_shared<Router>("R2");

    // Create link
    auto link = std::make_shared<Link>("Link1", 100.0, 10.0); // 100 Mbps, 10ms delay

    // Create interfaces
    auto interface1 = std::make_shared<Interface>("IF1", router1, link);
    auto interface2 = std::make_shared<Interface>("IF2", router2, link);

    // Connect interfaces to link
    link->connectInterface(interface1);
    link->connectInterface(interface2);

    // Add interfaces to routers
    router1->addInterface(interface1);
    router2->addInterface(interface2);

    // Set up routing
    router1->addRoute("R2", interface1);
    router2->addRoute("R1", interface2);

    // Create packet with payload
    auto payload = std::make_unique<DataPayload>("Hello, World!");
    auto packet = std::make_shared<Packet>("R1", "R2");
    packet->setPayload(std::move(payload));

    // Create and schedule initial event
    auto arrivalEvent = createPacketArrivalEvent(1.0, router1, packet, interface1);
    simulator.schedule(std::move(arrivalEvent));

    // Run simulation
    std::cout << "\n=== Running Simulation ===" << std::endl;
    simulator.run();

    std::cout << "\nSimulation completed!" << std::endl;
    return 0;
}
