#include "../include/event.hpp"
#include "../include/interface.hpp"
#include "../include/nodes/node.hpp"
#include "../include/packets/packet-time-tracker.hpp"
#include "../include/packets/packet.hpp"

#include <memory>
#include <string>

Event::Event(PacketTimeTracker *tracker) : tracker(tracker) {}

bool Event::operator<(const Event &other) const {
    // For min-heap: return true if this event should be processed AFTER other
    // Since we want earlier events to have higher priority, we return true if this event's time is greater
    return tracker->getCurrentTime() > other.tracker->getCurrentTime();
}

PacketArrivalEvent::PacketArrivalEvent(std::shared_ptr<Node> node, std::shared_ptr<Packet> packet,
                                       std::shared_ptr<Interface> iface, PacketTimeTracker *tracker)
    : Event(tracker), node_(node), packet_(packet), interface_(iface) {}

void PacketArrivalEvent::execute() {
    if (auto node = node_.lock()) {
        if (auto iface = interface_.lock()) {
            // handle packet arrival at the node
            node->receive(packet_, iface, tracker); // Use shared_ptr to manage packet memory
        }
    }
}

std::string PacketArrivalEvent::getDescription() const {
    auto node = node_.lock();
    auto iface = interface_.lock();
    std::string desc = "PacketArrivalEvent at node " + (node ? node->getID() : "INVALID") + " on interface " +
                       (iface ? iface->name : "INVALID");
    if (auto pkt = packet_) {
        desc += " | Packet: " + pkt->getID();
    } else {
        desc += " | Packet: INVALID";
    }
    return desc;
}

PacketDepartureEvent::PacketDepartureEvent(std::shared_ptr<Node> node, std::shared_ptr<Packet> packet,
                                           std::shared_ptr<Interface> iface, PacketTimeTracker *tracker)
    : Event(tracker), node_(node), packet_(packet), interface_(iface) {}

void PacketDepartureEvent::execute() {
    if (auto node = node_.lock()) {
        if (auto iface = interface_.lock()) {
            // Use the delay-aware interface method
            iface->sendPacket(packet_, tracker);
        }
    }
}

std::string PacketDepartureEvent::getDescription() const {
    auto node = node_.lock();
    auto iface = interface_.lock();

    std::string desc = "PacketDepartureEvent from node " + (node ? node->getID() : "INVALID") + " via interface " +
                       (iface ? iface->name : "INVALID");
    if (auto pkt = packet_) {
        desc += " | Packet: " + pkt->getID();
    } else {
        desc += " | Packet: INVALID";
    }
    return desc;
}

PacketTransmissionCompleteEvent::PacketTransmissionCompleteEvent(std::shared_ptr<Interface> iface,
                                                                 std::shared_ptr<Packet> packet,
                                                                 PacketTimeTracker *tracker)
    : Event(tracker), interface_(iface), packet_(packet) {}

void PacketTransmissionCompleteEvent::execute() {
    if (auto iface = interface_.lock()) {
        iface->receivePacket(packet_, tracker); // Deliver packet to the other end
    }
}

std::string PacketTransmissionCompleteEvent::getDescription() const {
    auto iface = interface_.lock();
    std::string desc = "PacketTransmissionCompleteEvent on interface " + (iface ? iface->name : "INVALID");
    if (auto pkt = packet_) {
        desc += " | Packet: " + pkt->getID();
    } else {
        desc += " | Packet: INVALID";
    }
    return desc;
}

// Factory functions for creating events
std::unique_ptr<Event> createPacketArrivalEvent(std::shared_ptr<Node> node, std::shared_ptr<Packet> packet,
                                                std::shared_ptr<Interface> iface, PacketTimeTracker *tracker) {
    return std::make_unique<PacketArrivalEvent>(node, packet, iface, tracker);
}

std::unique_ptr<Event> createPacketDepartureEvent(std::shared_ptr<Node> node, std::shared_ptr<Packet> packet,
                                                  std::shared_ptr<Interface> iface, PacketTimeTracker *tracker) {
    return std::make_unique<PacketDepartureEvent>(node, packet, iface, tracker);
}

std::unique_ptr<Event> createPacketTransmissionCompleteEvent(std::shared_ptr<Interface> iface,
                                                             std::shared_ptr<Packet> packet,
                                                             PacketTimeTracker *tracker) {
    return std::make_unique<PacketTransmissionCompleteEvent>(iface, packet, tracker);
}
