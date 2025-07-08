#pragma once

#include <memory>
#include <string>

// Forward declarations to avoid circular dependencies
class Node;
class Link;
class Packet;
class PacketTimeTracker;

class Interface : public std::enable_shared_from_this<Interface> {
  public:
    std::string name;         // Unique identifier for the interface
    std::weak_ptr<Node> node; // Weak pointer to the node this interface belongs to
    std::weak_ptr<Link> link; // Weak pointer to the link this interface is connected to

    Interface(const std::string &id, std::shared_ptr<Node> node, std::shared_ptr<Link> link);
    ~Interface();

    void sendPacket(std::shared_ptr<Packet> packet,
                    PacketTimeTracker *tracker); // Method to send a packet through this interface
    void receivePacket(std::shared_ptr<Packet> packet,
                       PacketTimeTracker *tracker); // Method to receive a packet from this interface

    bool isConnected() const;
    std::string getID() const; // Returns a unique identifier for the interface
};

// Note: Interface does not own Node or Link.
