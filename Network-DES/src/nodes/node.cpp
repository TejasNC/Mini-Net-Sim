#include "../../include/nodes/node.hpp"
#include "../../include/interface.hpp"

#include <algorithm>
#include <iostream>

Node::Node(const std::string &id) : id(id) {}

Node::~Node() = default;

void Node::addInterface(const std::shared_ptr<Interface> &interface) {
    interfaces.push_back(interface);
    std::cerr << "Interface " << interface->name << " added to node " << id << std::endl;
}

void Node::removeInterface(const std::shared_ptr<Interface> &interface) {
    auto it = std::find(interfaces.begin(), interfaces.end(), interface);
    if (it != interfaces.end()) {
        interfaces.erase(it);
        std::cerr << "Interface " << interface->name << " removed from node " << id << std::endl;
    }
}

std::string Node::getID() const { return id; }

std::vector<std::shared_ptr<Interface>> Node::getInterfaces() const { return interfaces; }
