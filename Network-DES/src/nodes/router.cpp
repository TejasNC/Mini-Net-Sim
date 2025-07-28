#include "../../include/nodes/router.hpp"
#include "../../include/interface.hpp"
#include "../../include/logger.hpp"
#include "../../include/packets/packet-time-tracker.hpp"
#include "../../include/packets/packet.hpp"
#include <iostream>

Router::Router(const std::string &id) : Node(id) {}

void Router::addRoute(const std::string &dstId, std::shared_ptr<Interface> interface) {
    routingTable[dstId] = interface;
    NetSim::Logger::log(NetSim::Logger::Level::DEBUG, 0.0, NetSim::Logger::ROUTER, getID(),
                        "Route added: [" + dstId + "] -> interface [" + interface->name + "]");
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
        NetSim::Logger::log(NetSim::Logger::Level::ERROR, 0.0, NetSim::Logger::ROUTER, getID(),
                            "Received null packet - memory ownership issue detected!");
        return;
    }
    double currTime = tracker->getCurrentTime();
    NetSim::Logger::log(NetSim::Logger::Level::DEBUG, currTime, NetSim::Logger::ROUTER, getID(),
                        "Packet received: [" + packet->srcNodeId + "] -> [" + packet->dstNodeId + "] via interface [" +
                            fromInterface->name + "]");

    // If packet is for this router (sanity check)
    if (packet->dstNodeId == getID()) {
        NetSim::Logger::log(NetSim::Logger::Level::WARNING, currTime, NetSim::Logger::ROUTER, getID(),
                            "Packet addressed directly to router - unusual routing scenario");
        return;
    }

    // Forward the packet
    send(packet, packet->dstNodeId, tracker);
}

void Router::send(std::shared_ptr<Packet> packet, const std::string dstId, PacketTimeTracker *tracker) {
    auto nextHopInterface = getNextHop(dstId);
    double currTime = tracker->getCurrentTime();

    if (nextHopInterface) {
        NetSim::Logger::log(NetSim::Logger::Level::INFO, currTime, NetSim::Logger::ROUTER, getID(),
                            "Forwarding packet to [" + dstId + "] via interface [" + nextHopInterface->name + "]");


        // Send packet to next hop
        nextHopInterface->sendPacket(packet, tracker);

        NetSim::Logger::log(NetSim::Logger::Level::DEBUG, currTime, NetSim::Logger::ROUTER, getID(),
                            "Packet successfully forwarded via interface [" + nextHopInterface->name + "]");

    } else {
        NetSim::Logger::log(NetSim::Logger::Level::WARNING, currTime, NetSim::Logger::ROUTER, getID(),
                            "Packet dropped: no route found to destination [" + dstId + "]");

        // TODO: Implement proper packet drop handling and statistics
    }
}

void Router::printRoutingTable() const {
    std::cerr << "Routing table for " << getID() << ":" << std::endl;
    for (const auto &entry : routingTable) {
        std::cerr << "  " << entry.first << " -> " << entry.second->name << std::endl;
    }
}
