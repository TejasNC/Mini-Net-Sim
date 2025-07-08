#include "../../include/nodes/router.hpp"
#include "../../include/interface.hpp"
#include "../../include/packets/packet-time-tracker.hpp"
#include "../../include/packets/packet.hpp"
#include <iostream>


Router::Router(const std::string &id) : Node(id) {}

void Router::addRoute(const std::string &dstId, std::shared_ptr<Interface> interface) {
    routingTable[dstId] = interface;
    std::cerr << "Route added: " << dstId << " -> " << interface->name << std::endl;
}

void Router::removeRoute(const std::string &dstId) {
    auto it = routingTable.find(dstId);
    if (it != routingTable.end()) {
        std::cerr << "Route removed: " << dstId << std::endl;
        routingTable.erase(it);
    } else {
        std::cerr << "Route not found: " << dstId << std::endl;
    }
}

std::shared_ptr<Interface> Router::getNextHop(const std::string &dstId) const {
    auto it = routingTable.find(dstId);
    if (it != routingTable.end()) {
        return it->second;
    }
    std::cerr << "No route found for destination: " << dstId << " at router " << getID() << std::endl;
    return nullptr;
}

void Router::receive(std::shared_ptr<Packet> packet, const std::shared_ptr<Interface> &fromInterface,
                     PacketTimeTracker *tracker) {

    if (!packet) {
        std::cerr << "Received null packet at router " << getID() << ". Sort out ownership issues!" << std::endl;
        return;
    }
    double currTime = tracker->getCurrentTime();
    std::cout << "[" << currTime << "] Router " << getID() << " received packet from " << packet->srcNodeId << " to "
              << packet->dstNodeId << " via " << fromInterface->name << std::endl;

    // If packet is for this router (sanity check)
    if (packet->dstNodeId == getID()) {
        std::cerr << "Packet has ended up at a router: " << getID() << std::endl;
        std::cout << "[" << currTime << "] "
                  << "Packet delivered to router " << getID() << std::endl;
        return;
    }

    // future versions:
    // add routing delay logic here

    // Forward the packet
    send(packet, packet->dstNodeId, tracker);
}

void Router::send(std::shared_ptr<Packet> packet, const std::string dstId, PacketTimeTracker *tracker) {
    auto nextHopInterface = getNextHop(dstId);
    double currTime = tracker->getCurrentTime();
    if (nextHopInterface) {
        std::cout << "[" << currTime << "] Router " << getID() << " forwarding packet to " << dstId << " via "
                  << nextHopInterface->name << std::endl;

        // future versions:
        // add routing delay logic here, e.g., tracker->addDelay(nextHopInterface->link->getDelay());
        // For now, we directly send the packet with the context

        nextHopInterface->sendPacket(packet, tracker); // Send with delay handling
        std::cout << "[" << currTime << "] "
                  << "Packet sent from router " << getID() << " to " << dstId << " via interface "
                  << nextHopInterface->name << std::endl;
    } else {
        std::cerr << "No route found from " << getID() << " to " << dstId << std::endl;
        std::cout << "[" << currTime << "] "
                  << "Packet dropped at router " << getID() << " as no route to " << dstId << std::endl;

        // TODO: fix memory issues with dropped packets in
        // PacketTimeTracker and Packet ownership

        // future versions:
        // handle packet drop logic here, e.g., incrementing a drop counter
        // or notifying the sender about the drop
    }
}

void Router::printRoutingTable() const {
    std::cerr << "Routing table for " << getID() << ":" << std::endl;
    for (const auto &entry : routingTable) {
        std::cerr << "  " << entry.first << " -> " << entry.second->name << std::endl;
    }
}
