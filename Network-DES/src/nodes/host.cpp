#include "../../include/nodes/host.hpp"
#include "../../include/interface.hpp"
#include "../../include/packets/packet-time-tracker.hpp"
#include "../../include/packets/packet.hpp"
#include "../../include/packets/payload.hpp"
#include <iostream>

Host::Host(const std::string &id) : Node(id) {}

void Host::receive(std::shared_ptr<Packet> packet, const std::shared_ptr<Interface> &fromInterface,
                   PacketTimeTracker *tracker) {

    if (!packet) {
        std::cerr << "Received null packet at host " << getID() << ". Sort out ownership issues!" << std::endl;
        return;
    }

    double currTime = tracker->getCurrentTime();
    std::cout << "[" << currTime << "] Host " << getID() << " received packet from " << packet->srcNodeId << " via "
              << fromInterface->name << std::endl;

    // Check if packet is for this host
    if (packet->dstNodeId == getID()) {
        std::cout << "[" << currTime << "] Packet successfully delivered to host " << getID() << std::endl;

        // Process the packet (in a real implementation, this would extract and process the payload)
        if (packet->payload) {
            std::cout << "[" << currTime << "] Host " << getID()
                      << " processing packet payload (size: " << packet->sizeBytes() << " bytes)" << std::endl;
        }

        std::cout << "[" << currTime << "] Packet consumption complete at host " << getID() << std::endl;
    } else {
        std::cerr << "Host " << getID() << " received packet not destined for it (dst: " << packet->dstNodeId << ")"
                  << std::endl;
    }
}

void Host::send(std::shared_ptr<Packet> packet, const std::string dstId, PacketTimeTracker *tracker) {
    double currTime = tracker->getCurrentTime();

    // Hosts typically have only one interface, so use the first available one
    if (interfaces.empty()) {
        std::cerr << "Host " << getID() << " has no interfaces to send packet" << std::endl;
        return;
    }

    auto interface = interfaces[0]; // Use first interface

    std::cout << "[" << currTime << "] Host " << getID() << " sending packet to " << dstId << " via interface "
              << interface->name << std::endl;

    // Send packet through the interface
    interface->sendPacket(packet, tracker);

    std::cout << "[" << currTime << "] Packet sent from host " << getID() << " to " << dstId << " via interface "
              << interface->name << std::endl;
}

void Host::generatePacket(const std::string &dstId, const std::string &message, PacketTimeTracker *tracker) {
    double currTime = tracker->getCurrentTime();

    std::cout << "[" << currTime << "] Host " << getID() << " generating packet for " << dstId << std::endl;

    // Create packet
    auto packet = std::make_shared<Packet>(getID(), dstId);

    // Create payload with the message
    auto payload = std::make_unique<DataPayload>(message);
    packet->setPayload(std::move(payload));

    std::cout << "[" << currTime << "] Host " << getID() << " created packet " << packet->getID()
              << " (size: " << packet->sizeBytes() << " bytes)" << std::endl;

    // Send the packet
    send(packet, dstId, tracker);
}
