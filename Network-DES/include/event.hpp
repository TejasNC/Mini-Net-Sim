#pragma once

#include <memory>
#include <string>

// Forward declarations
class SimulatorContext;
class Node;
class Packet;
class Interface;

class Event {
public:
    double time;

    Event(double t);
    virtual ~Event() = default;

    virtual void execute(SimulatorContext& ctx) = 0;
    virtual std::string getDescription() const = 0;

    // Comparison operators for priority queue
    bool operator<(const Event &other) const { return time > other.time; } // For min-heap

    // Guard against copying
    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

};

class PacketArrivalEvent : public Event {
private:
    std::weak_ptr<Node> node_;
    std::shared_ptr<Packet> packet_;
    std::weak_ptr<Interface> interface_;

public:
    PacketArrivalEvent(double t, std::shared_ptr<Node> node,
                      std::shared_ptr<Packet> packet, std::shared_ptr<Interface> iface);

    void execute(SimulatorContext& ctx) override;
    std::string getDescription() const override;
};

class PacketDepartureEvent : public Event {
private:
    std::weak_ptr<Node> node_;
    std::shared_ptr<Packet> packet_;
    std::weak_ptr<Interface> interface_;

public:
    PacketDepartureEvent(double t, std::shared_ptr<Node> node,
                       std::shared_ptr<Packet> packet, std::shared_ptr<Interface> iface);

    void execute(SimulatorContext& ctx) override;
    std::string getDescription() const override;
};

class PacketTransmissionCompleteEvent : public Event {
private:
    std::weak_ptr<Interface> interface_;
    std::shared_ptr<Packet> packet_;


public:
    PacketTransmissionCompleteEvent(double t, std::shared_ptr<Interface> iface,
                                  std::shared_ptr<Packet> packet);

    void execute(SimulatorContext& ctx) override;
    std::string getDescription() const override;
};

// Factory functions for creating events
std::unique_ptr<Event> createPacketArrivalEvent(double t, std::shared_ptr<Node> node,
                                               std::shared_ptr<Packet> packet, std::shared_ptr<Interface> iface);

std::unique_ptr<Event> createPacketDepartureEvent(double t, std::shared_ptr<Node> node,
                                                 std::shared_ptr<Packet> packet, std::shared_ptr<Interface> iface);

std::unique_ptr<Event> createPacketTransmissionCompleteEvent(double t, std::shared_ptr<Interface> iface,
                                                           std::shared_ptr<Packet> packet);
