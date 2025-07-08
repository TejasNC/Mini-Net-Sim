#include "../include/link.hpp"
#include "../include/interface.hpp"
#include <iostream>

Link::Link(const std::string &id, double bandwidth, double delay)
    : name(id), bandwidthMbps(bandwidth), delayMs(delay) {}

Link::~Link() = default;

void Link::connectInterface(std::shared_ptr<Interface> interface) {
    if (!interface1) {
        interface1 = interface;
        std::cout << "Interface " << interface->name << " connected to link " << name << " (endpoint 1)" << std::endl;
    } else if (!interface2) {
        interface2 = interface;
        std::cout << "Interface " << interface->name << " connected to link " << name << " (endpoint 2)" << std::endl;
    } else {
        std::cout << "Error: Link " << name << " already has two interfaces connected" << std::endl;
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

bool Link::isFullyConnected() const {
    return interface1 && interface2;
}

double Link::getBandwidth() const {
    return bandwidthMbps;
}

double Link::getDelay() const {
    return delayMs;
}

std::string Link::getID() const {
    return name;
}
