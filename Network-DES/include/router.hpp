#pragma once

#include "node.hpp"
#include <unordered_map>
#include <memory>

class Router : public Node {

  private:
    std::unordered_map<std::string, std::shared_ptr<Interface>> routingTable; // Maps destination IDs to interfaces

  public:
    Router(const std::string &id);
    ~Router() override = default;

    // guards
    Router(const Router&) = delete;
    Router& operator=(const Router&) = delete;

    // Router methods
    void addRoute(const std::string &dstId, std::shared_ptr<Interface> interface);   // Adds a route to the routing table
    void removeRoute(const std::string &dstId);                                      // Removes a route from the routing table
    std::shared_ptr<Interface> getNextHop(const std::string &dstId) const;           // Gets the next hop interface for a destination ID
    void receive(std::shared_ptr<Packet> packet, const std::shared_ptr<Interface> &fromInterface) override; // Receives a packet from a specific interface
    void send(std::shared_ptr<Packet> packet, const std::string dstId) override;                     // Sends a packet to a specific destination ID

    void printRoutingTable() const;
};
