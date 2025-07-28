#pragma once

#include <memory>
#include <string>

class PacketTimeTracker;
class Node;
class Packet;
class Interface;

class Event {
  public:
    PacketTimeTracker *tracker; // Pointer to the packet time tracker

    Event(PacketTimeTracker *timeTracker);
    virtual ~Event() = default;

    virtual void execute() = 0;
    virtual std::string getDescription() const = 0;

    // Comparison operators for priority queue
    bool operator<(const Event &other) const; // For min-heap

    // Guard against copying
    Event(const Event &) = delete;
    Event &operator=(const Event &) = delete;
};

class PacketArrivalEvent : public Event {
  private:
    std::weak_ptr<Node> node_;
    std::shared_ptr<Packet> packet_;
    std::weak_ptr<Interface> interface_;

  public:
    PacketArrivalEvent(std::shared_ptr<Node> node, std::shared_ptr<Packet> packet, std::shared_ptr<Interface> iface,
                       PacketTimeTracker *tracker);

    void execute() override;
    std::string getDescription() const override;
};

class PacketDepartureEvent : public Event {
  private:
    std::weak_ptr<Node> node_;
    std::shared_ptr<Packet> packet_;
    std::weak_ptr<Interface> interface_;

  public:
    PacketDepartureEvent(std::shared_ptr<Node> node, std::shared_ptr<Packet> packet, std::shared_ptr<Interface> iface,
                         PacketTimeTracker *tracker);

    void execute() override;
    std::string getDescription() const override;
};

// TODO: change needed => review this (the construct does not feel right. there should be node instead of interface)
class PacketTransmissionCompleteEvent : public Event {
  private:
    std::weak_ptr<Interface> interface_;
    std::shared_ptr<Packet> packet_;

  public:
    PacketTransmissionCompleteEvent(std::shared_ptr<Interface> iface, std::shared_ptr<Packet> packet,
                                    PacketTimeTracker *tracker);

    void execute() override;
    std::string getDescription() const override;
};

// Factory functions for creating events
std::unique_ptr<Event> createPacketArrivalEvent(std::shared_ptr<Node> node, std::shared_ptr<Packet> packet,
                                                std::shared_ptr<Interface> iface, PacketTimeTracker *tracker);

std::unique_ptr<Event> createPacketDepartureEvent(std::shared_ptr<Node> node, std::shared_ptr<Packet> packet,
                                                  std::shared_ptr<Interface> iface, PacketTimeTracker *tracker);

std::unique_ptr<Event> createPacketTransmissionCompleteEvent(std::shared_ptr<Interface> iface,
                                                             std::shared_ptr<Packet> packet,
                                                             PacketTimeTracker *tracker);
