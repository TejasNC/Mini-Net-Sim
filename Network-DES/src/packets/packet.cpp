#include "../../include/packets/packet.hpp"
#include "../../include/packets/payload.hpp"
#include <memory>

Packet::Packet(const std::string& src, const std::string& dst)
    : srcNodeId(src), dstNodeId(dst), payload(nullptr) {
        this->name = "Packet_" + src + "_to_" + dst; // Generate a unique name for the packet
    }

Packet::~Packet() {
    // Unique pointer will automatically clean up the payload
}

void Packet::setPayload(std::unique_ptr<Payload> p) {
    this->payload = std::move(p);
}

size_t Packet::sizeBytes() const {
    if (payload) {
        return payload->sizeBytes();
    }
    return 0; // Return 0 if no payload is set
}

std::string Packet::getID() const {
    return name;
}
