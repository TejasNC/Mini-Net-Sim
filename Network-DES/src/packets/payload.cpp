#include "../../include/packets/payload.hpp"

DataPayload::DataPayload(const std::string &data) : data(data) {}

size_t DataPayload::sizeBytes() const {
    return data.size() * sizeof(char); // Calculate size in bytes based on the string length
}

std::unique_ptr<Payload> DataPayload::clone() const {
    return std::make_unique<DataPayload>(*this);
}

