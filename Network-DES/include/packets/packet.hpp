#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "payload.hpp"

class Packet {

  public:
    std::string name;      // Unique identifier for the packet
    std::string srcNodeId; // Source node ID
    std::string dstNodeId; // Destination node ID

    // version 1.0
    // std::string protocol; // Protocol type (e.g., TCP, UDP)

    std::unique_ptr<Payload> payload; // Unique pointer to the payload data

    Packet(const std::string &src, const std::string &dst);
    ~Packet();

    void setPayload(std::unique_ptr<Payload> payload);
    // This method takes ownership of the payload via unique_ptr

    size_t sizeBytes() const;

    std::string getID() const; // Returns a unique identifier for the packet

    // version 1.0
    // std::vector<PacketHeader*> headers; // Vector of packet headers
    // void addHeader(PacketHeader* header);
    // void removeHeader(PacketHeader* header);
};

// Note: Payload is owned by the Packet class
