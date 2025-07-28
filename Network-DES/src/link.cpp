#include "../include/link.hpp"
#include "../include/interface.hpp"
#include "../include/logger.hpp"


Link::Link(const std::string &id, double bandwidth, double delay)
    : name(id), bandwidthMbps(bandwidth), delayMs(delay) {}

Link::~Link() = default;

void Link::connectInterface(std::shared_ptr<Interface> interface) {
    if (!interface1) {
        interface1 = interface;
        NetSim::Logger::log(NetSim::Logger::Level::DEBUG, 0.0,
                            "Interface " + interface->name + " connected to link " + name + " (endpoint 1)");
    } else if (!interface2) {
        interface2 = interface;
        NetSim::Logger::log(NetSim::Logger::Level::DEBUG, 0.0,
                            "Interface " + interface->name + " connected to link " + name + " (endpoint 2)");
    } else {
        NetSim::Logger::log(NetSim::Logger::Level::ERROR, 0.0,
                            "Link " + name + " already has two interfaces connected");
    }
}

std::shared_ptr<Interface> Link::getOtherInterface(std::shared_ptr<Interface> interface) {
    if (interface1 == interface) {
        return interface2;
    } else if (interface2 == interface) {
        return interface1;
    }
    return nullptr;
}

bool Link::isFullyConnected() const { return interface1 && interface2; }

double Link::getBandwidth() const { return bandwidthMbps; }

double Link::getDelay() const { return delayMs; }

std::string Link::getID() const { return name; }
