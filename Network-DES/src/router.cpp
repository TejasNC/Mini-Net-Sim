#include "../include/router.hpp"
#include "../include/interface.hpp"
#include "../include/packets/packet.hpp"
#include <iostream>

Router::Router(const std::string &id) : Node(id) {}

void Router::addRoute(const std::string &dstId, std::shared_ptr<Interface> interface) {
    routingTable[dstId] = interface;
    std::cout << "Route added: " << dstId << " -> " << interface->name << std::endl;
}

void Router::removeRoute(const std::string &dstId) {
    auto it = routingTable.find(dstId);
    if (it != routingTable.end()) {
        std::cout << "Route removed: " << dstId << std::endl;
        routingTable.erase(it);
    }
}

std::shared_ptr<Interface> Router::getNextHop(const std::string &dstId) const {
    auto it = routingTable.find(dstId);
    if (it != routingTable.end()) {
        return it->second;
    }
    return nullptr;
}

void Router::receive(std::shared_ptr<Packet> packet, const std::shared_ptr<Interface> &fromInterface) {
    std::cout << "Router " << getID() << " received packet from " << packet->srcNodeId
              << " to " << packet->dstNodeId << " via " << fromInterface->name << std::endl;

    // If packet is for this router
    if (packet->dstNodeId == getID()) {
        std::cout << "Packet delivered to router " << getID() << std::endl;
        return;
    }

    // Forward the packet
    send(packet, packet->dstNodeId);
}

void Router::send(std::shared_ptr<Packet> packet, const std::string dstId) {
    auto nextHopInterface = getNextHop(dstId);
    if (nextHopInterface) {
        std::cout << "Router " << getID() << " forwarding packet to " << dstId
                  << " via " << nextHopInterface->name << std::endl;
        nextHopInterface->sendPacket(packet);
    } else {
        std::cout << "No route found from " << getID() << " to " << dstId << std::endl;
    }
}

void Router::printRoutingTable() const {
    std::cout << "Routing table for " << getID() << ":" << std::endl;
    for (const auto &entry : routingTable) {
        std::cout << "  " << entry.first << " -> " << entry.second->name << std::endl;
    }
}
