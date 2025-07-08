#include "../include/event.hpp"
#include "../include/interface.hpp"
#include "../include/node.hpp"
#include "../include/packets/packet.hpp"
#include "../include/simulator.hpp"

#include <memory>
#include <string>

// constructor
Event::Event(double t) : time(t) {}

PacketArrivalEvent::PacketArrivalEvent(double t, std::shared_ptr<Node> node, std::shared_ptr<Packet> packet,
                                       std::shared_ptr<Interface> iface)
    : Event(t), node_(node), packet_(packet), interface_(iface) {}

void PacketArrivalEvent::execute(SimulatorContext &ctx) {
    if (auto node = node_.lock()) {
        if (auto iface = interface_.lock()) {
            // node->receive(packet_.get(), iface.get());
            node->receive(packet_, iface); // Use shared_ptr to manage packet memory
        }
    }
}

std::string PacketArrivalEvent::getDescription() const {
    auto node = node_.lock();
    auto iface = interface_.lock();

    return "PacketArrivalEvent at node " +
           (node ? node->getID() : "INVALID") +
           " on interface " +
           (iface ? iface->name : "INVALID");
}

PacketDepartureEvent::PacketDepartureEvent(double t, std::shared_ptr<Node> node, std::shared_ptr<Packet> packet,
                                           std::shared_ptr<Interface> iface)
    : Event(t), node_(node), packet_(packet), interface_(iface) {}

void PacketDepartureEvent::execute(SimulatorContext &ctx) {
    if (auto node = node_.lock()) {
        if (auto iface = interface_.lock()) {
            // Use the delay-aware interface method
            iface->sendPacketWithContext(packet_, &ctx);
        }
    }
}

std::string PacketDepartureEvent::getDescription() const {
    auto node = node_.lock();
    auto iface = interface_.lock();

    return "PacketDepartureEvent from node " +
           (node ? node->getID() : "INVALID") +
           " via interface " +
           (iface ? iface->name : "INVALID");
}

PacketTransmissionCompleteEvent::PacketTransmissionCompleteEvent(double t, std::shared_ptr<Interface> iface,
                                                                 std::shared_ptr<Packet> packet)
    : Event(t), interface_(iface), packet_(packet) {}

void PacketTransmissionCompleteEvent::execute(SimulatorContext &ctx) {
    if (auto iface = interface_.lock()) {
        iface->receivePacket(packet_); // Deliver packet to the other end
    }
}

std::string PacketTransmissionCompleteEvent::getDescription() const {
    auto iface = interface_.lock();

    return "PacketTransmissionCompleteEvent on interface " +
           (iface ? iface->name : "INVALID");
}

// Factory functions for creating events
std::unique_ptr<Event> createPacketArrivalEvent(double t, std::shared_ptr<Node> node,
                                               std::shared_ptr<Packet> packet, std::shared_ptr<Interface> iface) {
    return std::make_unique<PacketArrivalEvent>(t, node, packet, iface);
}

std::unique_ptr<Event> createPacketDepartureEvent(double t, std::shared_ptr<Node> node,
                                                 std::shared_ptr<Packet> packet, std::shared_ptr<Interface> iface) {
    return std::make_unique<PacketDepartureEvent>(t, node, packet, iface);
}

std::unique_ptr<Event> createPacketTransmissionCompleteEvent(double t, std::shared_ptr<Interface> iface,
                                                           std::shared_ptr<Packet> packet) {
    return std::make_unique<PacketTransmissionCompleteEvent>(t, iface, packet);
}
