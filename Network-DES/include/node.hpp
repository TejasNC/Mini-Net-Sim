#pragma once

#include <string>
#include <vector>

// Forward declarations to avoid circular dependencies
class Interface;
class Packet;

#include <memory>

class Node {

  protected:
    std::string id;
    std::vector<std::shared_ptr<Interface>> interfaces; // Node holds shared ownership of Interface objects.

  public:
    Node(const std::string &id);
    virtual ~Node();

    // Node methods
    virtual void receive(std::shared_ptr<Packet> packet, const std::shared_ptr<Interface> &fromInterface) = 0; // Receives a packet from a specific interface
    virtual void send(std::shared_ptr<Packet> packet, const std::string dstId) = 0;           // Sends a packet to a specific destination ID
    void addInterface(const std::shared_ptr<Interface> &interface);           // Adds an interface to the node
    void removeInterface(const std::shared_ptr<Interface> &interface);        // Removes an interface from the node

    std::string getID() const;
    std::vector<std::shared_ptr<Interface>> getInterfaces() const;
};
