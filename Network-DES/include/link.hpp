#pragma once

#include <memory>
#include <string>

// Forward declarations to avoid circular dependencies
class Interface;

class Link {

  public:
    std::string name; // Unique identifier for the link

    double bandwidthMbps; // Bandwidth of the link in Mbps
    double delayMs;       // Delay of the link in milliseconds

    std::shared_ptr<Interface> interface1; // Shared pointer to the first interface
    std::shared_ptr<Interface> interface2; // Shared pointer to the second interface

    //   for version 1.0
    //   bool isDuplex;        // Indicates if the link is duplex (true)

    Link(const std::string &id, double bandwidth, double delay);
    ~Link();

    // Link management methods
    void connectInterface(std::shared_ptr<Interface> interface);
    std::shared_ptr<Interface> getOtherInterface(std::shared_ptr<Interface> interface);
    bool isFullyConnected() const;
    double getBandwidth() const;
    double getDelay() const;
    std::string getID() const; // Returns a unique identifier for the link
};

// Note: Link shares ownership of interfaces it connects.
