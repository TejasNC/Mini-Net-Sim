#pragma once

#include "node.hpp"
#include <memory>

class Host : public Node {

  public:
    Host(const std::string &id);
    ~Host() override = default;

    // Guards against copying
    Host(const Host &) = delete;
    Host &operator=(const Host &) = delete;

    // Host methods
    void receive(std::shared_ptr<Packet> packet, const std::shared_ptr<Interface> &fromInterface,
                 PacketTimeTracker *tracker) override; // Receives a packet (final destination)
    void send(std::shared_ptr<Packet> packet, const std::string dstId,
              PacketTimeTracker *tracker) override; // Sends a packet from this host

    // Host-specific methods
    void generatePacket(const std::string &dstId, const std::string &message, PacketTimeTracker *tracker);
};
