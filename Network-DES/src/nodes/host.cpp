#include "../../include/nodes/host.hpp"
#include "../../include/interface.hpp"
#include "../../include/logger.hpp"
#include "../../include/packets/packet-time-tracker.hpp"
#include "../../include/packets/packet.hpp"
#include "../../include/packets/payload.hpp"

Host::Host(const std::string &id) : Node(id) {}

void Host::receive(std::shared_ptr<Packet> packet, const std::shared_ptr<Interface> &fromInterface,
                   PacketTimeTracker *tracker) {

    if (!packet) {
        NetSim::Logger::log(NetSim::Logger::Level::ERROR, 0.0, NetSim::Logger::HOST, getID(),
                            "Received null packet - memory ownership issue detected!");
        return;
    }

    double currTime = tracker->getCurrentTime();
    NetSim::Logger::log(NetSim::Logger::Level::DEBUG, currTime, NetSim::Logger::HOST, getID(),
                        "Packet received from [" + packet->srcNodeId + "] via interface [" + fromInterface->name + "]");


    // Check if packet is for this host
    if (packet->dstNodeId == getID()) {
        NetSim::Logger::log(NetSim::Logger::Level::INFO, currTime, NetSim::Logger::HOST, getID(),
                            "Packet successfully delivered! Processing payload...");

        // Process the packet payload
        if (packet->payload) {
            NetSim::Logger::log(NetSim::Logger::Level::DEBUG, currTime, NetSim::Logger::PACKET, packet->getID(),
                                "Processing payload (size: " + std::to_string(packet->sizeBytes()) + " bytes)");
        }

        NetSim::Logger::log(NetSim::Logger::Level::DEBUG, currTime, NetSim::Logger::HOST, getID(),
                            "Packet processing completed successfully");
    } else {
        NetSim::Logger::log(NetSim::Logger::Level::ERROR, currTime, NetSim::Logger::HOST, getID(),
                            "Packet routing error: received packet destined for [" + packet->dstNodeId +
                                "] instead of [" + getID() + "]");
    }

}

void Host::send(std::shared_ptr<Packet> packet, const std::string dstId, PacketTimeTracker *tracker) {
    double currTime = tracker->getCurrentTime();

    // Hosts typically have only one interface, so use the first available one
    if (interfaces.empty()) {
        NetSim::Logger::log(NetSim::Logger::Level::ERROR, currTime, NetSim::Logger::HOST, getID(),
                            "Cannot send packet: no network interfaces available");
        return;
    }

    auto interface = interfaces[0]; // Use first interface

    NetSim::Logger::log(NetSim::Logger::Level::INFO, currTime, NetSim::Logger::HOST, getID(),
                        "Sending packet to [" + dstId + "] via interface [" + interface->name + "]");


    // Send packet through the interface
    interface->sendPacket(packet, tracker);

    NetSim::Logger::log(NetSim::Logger::Level::DEBUG, currTime, NetSim::Logger::HOST, getID(),
                        "Packet successfully transmitted to interface [" + interface->name + "]");

}

void Host::generatePacket(const std::string &dstId, const std::string &message, PacketTimeTracker *tracker) {
    double currTime = tracker->getCurrentTime();

    NetSim::Logger::log(NetSim::Logger::Level::INFO, currTime, NetSim::Logger::HOST, getID(),
                        "Generating new packet for destination [" + dstId + "]");


    // Create packet
    auto packet = std::make_shared<Packet>(getID(), dstId);

    // Create payload with the message
    auto payload = std::make_unique<DataPayload>(message);
    packet->setPayload(std::move(payload));

    NetSim::Logger::log(NetSim::Logger::Level::DEBUG, currTime, NetSim::Logger::PACKET, packet->getID(),
                        "Created packet (size: " + std::to_string(packet->sizeBytes()) + " bytes, payload: \"" +
                            message + "\")");

    // Send the packet
    send(packet, dstId, tracker);

}
