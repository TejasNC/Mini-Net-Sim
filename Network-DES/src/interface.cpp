#include "../include/interface.hpp"
#include "../include/link.hpp"
#include "../include/node.hpp"
#include "../include/packets/packet.hpp"
#include "../include/simulator.hpp"
#include "../include/event.hpp"
#include <iostream>

Interface::Interface(const std::string &id, std::shared_ptr<Node> node, std::shared_ptr<Link> link)
    : name(id), node(node), link(link) {
    std::cout << "Interface " << name << " created for node " << node->getID()
              << " and link " << link->getID() << std::endl;
}

Interface::~Interface() = default;

std::string Interface::getID() const { return name;}

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

void Interface::sendPacket(std::shared_ptr<Packet> packet) {
    sendPacketWithContext(packet, nullptr);
}

void Interface::sendPacketWithContext(std::shared_ptr<Packet> packet, SimulatorContext* ctx) {
    if (!isConnected()) {
        std::cerr << "Cannot send packet from " << name << ": Interface is not connected." << std::endl;
        return;
    }

    auto linkPtr = link.lock();
    if (!linkPtr) {
        std::cerr << "Cannot send packet: Link has been destroyed." << std::endl;
        return;
    }

    std::cout << "Interface " << name << " sending packet " << packet->srcNodeId << "->" << packet->dstNodeId
              << " through link " << linkPtr->getID() << std::endl;

    // Get the other interface on the link
    auto otherInterface = linkPtr->getOtherInterface(shared_from_this());
    if (!otherInterface) {
        std::cerr << "No destination interface found on link " << linkPtr->getID() << std::endl;
        return;
    }

    if (ctx) {
        // Calculate transmission delay based on packet size and link bandwidth
        double transmissionDelay = ctx->computeTransmissionDelay(packet->sizeBytes(), linkPtr->getBandwidth());

        // Calculate propagation delay
        double propagationDelay = ctx->computePropagationDelay(linkPtr->getDelay());

        // Total delay
        double totalDelay = transmissionDelay + propagationDelay;

        std::cout << "Scheduling packet transmission completion in " << totalDelay << " seconds" << std::endl;

        // Schedule a transmission complete event
        auto transmissionEvent = createPacketTransmissionCompleteEvent(
            ctx->getCurrentTime() + totalDelay,
            otherInterface,
            packet
        );

        ctx->schedule(std::move(transmissionEvent));
    } else {
        // Fallback: immediate delivery (for backwards compatibility)
        std::cout << "Warning: No simulator context provided, delivering packet immediately" << std::endl;
        otherInterface->receivePacket(packet);
    }
}


void Interface::receivePacket(std::shared_ptr<Packet> packet) {
    if (!isConnected()) {
        std::cerr << "Cannot receive packet at " << name << ": Interface is not connected." << std::endl;
        return;
    }

    auto nodePtr = node.lock();
    if (!nodePtr) {
        std::cerr << "Cannot deliver packet: Node has been destroyed." << std::endl;
        return;
    }

    std::cout << "Interface " << name << " received packet " << packet->getID() << std::endl;

    // Deliver packet to the node
    nodePtr->receive(packet, shared_from_this());
}

