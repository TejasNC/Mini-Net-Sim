#pragma once

#include <string>
#include <memory>

struct Payload {
    virtual size_t sizeBytes() const = 0;
    virtual ~Payload() {}
    virtual std::unique_ptr<Payload> clone() const = 0;
};

struct DataPayload : public Payload {
    std::string data;
    DataPayload(const std::string &data);
    size_t sizeBytes() const override;
    std::unique_ptr<Payload> clone() const override;
    ~DataPayload() override = default;
    DataPayload(const DataPayload&) = default; // Default copy constructor
};
