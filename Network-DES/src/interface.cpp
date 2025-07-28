#include "../include/interface.hpp"
#include "../include/event.hpp"
#include "../include/link.hpp"
#include "../include/logger.hpp"
#include "../include/nodes/node.hpp"
#include "../include/packets/packet-time-tracker.hpp"
#include "../include/packets/packet.hpp"
#include <iostream>

Interface::Interface(const std::string &id, std::shared_ptr<Node> node, std::shared_ptr<Link> link)
    : name(id), node(node), link(link) {
    // std::cerr << "Interface " << name << " created for node " << node->getID() << " and link " << link->getID()
    //           << std::endl;
}

Interface::~Interface() = default;

std::string Interface::getID() const { return name; }

bool Interface::isConnected() const {
    if (link.expired()) {
        std::cerr << "Interface " << name << " is not connected to any link." << std::endl;
        return false;
    }
    if (node.expired()) {
        std::cerr << "Interface " << name << " is not connected to any node." << std::endl;
        return false;
    }
    return true;
}

void Interface::sendPacket(std::shared_ptr<Packet> packet, PacketTimeTracker *tracker) {

    if (!isConnected()) {
        NetSim::Logger::log(NetSim::Logger::Level::ERROR, 0.0, NetSim::Logger::INTERFACE, name,
                            "Cannot send packet: interface not connected to link");
        return;
    }

    auto linkPtr = link.lock(); // Safe to call since isConnected() already validated the link

    double currentTime = tracker ? tracker->getCurrentTime() : 0.0;
    NetSim::Logger::log(NetSim::Logger::Level::DEBUG, currentTime, NetSim::Logger::INTERFACE, name,
                        "Transmitting packet [" + packet->getID() + "] via link [" + linkPtr->getID() + "]");


    // Get the other interface on the link
    auto otherInterface = linkPtr->getOtherInterface(shared_from_this()); // debug this

    if (tracker) {
        // Calculate transmission delay based on packet size and link bandwidth
        double transmissionDelay = tracker->computeTransmissionDelay(packet->sizeBytes(), linkPtr->getBandwidth());

        // Calculate propagation delay
        double propagationDelay = tracker->computePropagationDelay(linkPtr->getDelay());

        // Total delay
        double totalDelay = transmissionDelay + propagationDelay;

        NetSim::Logger::log(NetSim::Logger::Level::DEBUG, currentTime, NetSim::Logger::LINK, linkPtr->getID(),
                            "Delays: transmission=" + std::to_string(transmissionDelay) + "ms, propagation=" +
                                std::to_string(propagationDelay) + "ms, total=" + std::to_string(totalDelay) + "ms");

        // update packet's transmission time
        tracker->currentTime += totalDelay;

        // Schedule a transmission complete event
        auto transmissionEvent = createPacketTransmissionCompleteEvent(otherInterface, packet, tracker);

        tracker->schedule(std::move(transmissionEvent)); // transfer ownership of the event

    } else {
        // Fallback: immediate delivery (for backwards compatibility)
        NetSim::Logger::log(NetSim::Logger::Level::WARNING, currentTime, NetSim::Logger::INTERFACE, name,
                            "No time tracker provided - delivering packet immediately (unrealistic)");
        otherInterface->receivePacket(packet, nullptr);
    }

}

void Interface::receivePacket(std::shared_ptr<Packet> packet, PacketTimeTracker *tracker) {

    if (!isConnected()) {
        NetSim::Logger::log(NetSim::Logger::Level::ERROR, 0.0, NetSim::Logger::INTERFACE, name,
                            "Cannot receive packet: interface not connected");
        return;
    }

    auto nodePtr = node.lock();

    double currentTime = tracker ? tracker->getCurrentTime() : 0.0;
    NetSim::Logger::log(NetSim::Logger::Level::DEBUG, currentTime, NetSim::Logger::INTERFACE, name,
                        "Packet [" + packet->getID() + "] received, delivering to node [" + nodePtr->getID() + "]");

    // Deliver packet to the node
    nodePtr->receive(packet, shared_from_this(), tracker);
}
