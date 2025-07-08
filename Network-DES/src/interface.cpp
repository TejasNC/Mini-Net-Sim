#include "../include/interface.hpp"
#include "../include/event.hpp"
#include "../include/link.hpp"
#include "../include/nodes/node.hpp"
#include "../include/packets/packet-time-tracker.hpp"
#include "../include/packets/packet.hpp"
#include <iostream>

Interface::Interface(const std::string &id, std::shared_ptr<Node> node, std::shared_ptr<Link> link)
    : name(id), node(node), link(link) {
    std::cerr << "Interface " << name << " created for node " << node->getID() << " and link " << link->getID()
              << std::endl;
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
        std::cerr << "Cannot send packet from " << name << ": Interface is not connected." << std::endl;
        return;
    }

    auto linkPtr = link.lock(); // Safe to call since isConnected() already validated the link

    std::cout << "Interface " << name << " sending packet " << packet->srcNodeId << "->" << packet->dstNodeId
              << " through link " << linkPtr->getID() << std::endl;

    // Get the other interface on the link
    auto otherInterface = linkPtr->getOtherInterface(shared_from_this()); // debug this

    if (tracker) {
        // Calculate transmission delay based on packet size and link bandwidth
        // simulates packet fragmentation and transmission (somewhat simplified) => add delay for each fragment in
        // future versions
        double transmissionDelay = tracker->computeTransmissionDelay(packet->sizeBytes(), linkPtr->getBandwidth());

        // Calculate propagation delay
        double propagationDelay = tracker->computePropagationDelay(linkPtr->getDelay());

        // Total delay
        double totalDelay = transmissionDelay + propagationDelay;

        std::cout << "Scheduling packet transmission completion in " << totalDelay << "ms" << std::endl;

        // update packet's transmission time
        tracker->currentTime += totalDelay;

        // Schedule a transmission complete event
        auto transmissionEvent = createPacketTransmissionCompleteEvent(otherInterface, packet, tracker);

        tracker->schedule(std::move(transmissionEvent)); // transfer ownership of the event

    } else {
        // Fallback: immediate delivery (for backwards compatibility)
        std::cout << "Warning: No packet time tracker provided. Delivering packet immediately."
                  << std::endl; // for now, put this message in both streams
        std::cerr << "Warning: No packet time tracker provided. Delivering packet immediately." << std::endl;
        otherInterface->receivePacket(packet, nullptr);
    }

    return;
}

void Interface::receivePacket(std::shared_ptr<Packet> packet, PacketTimeTracker *tracker) {

    if (!isConnected()) {
        std::cerr << "Cannot receive packet at " << name << ": Interface is not connected." << std::endl;
        return;
    }

    auto nodePtr = node.lock();

    std::cout << "Interface " << name << " received packet " << packet->getID() << std::endl;

    // Deliver packet to the node
    nodePtr->receive(packet, shared_from_this(), tracker);
}
