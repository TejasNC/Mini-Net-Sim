#include "../include/simulator.hpp"
#include "../include/logger.hpp"
#include "../include/packets/packet-time-tracker.hpp"

Simulator::Simulator() : eventQueue(), currentTime(0.0) {}

void Simulator::schedule(std::unique_ptr<Event> event) {

    if (event) {
        if (event->tracker->getCurrentTime() < currentTime) {
            NetSim::Logger::log(NetSim::Logger::Level::WARNING, currentTime, NetSim::Logger::SIMULATOR, "SIM",
                "Scheduling event in the past (event: " + std::to_string(event->tracker->getCurrentTime()) +
                "s, current: " + std::to_string(currentTime) + "s)");
        }
        eventQueue.push(std::move(event)); // Use move semantics to transfer ownership
    } else {
        NetSim::Logger::log(NetSim::Logger::Level::ERROR, currentTime, NetSim::Logger::SIMULATOR, "SIM",
            "Attempted to schedule null event");
    }
}

void Simulator::run() {

    NetSim::Logger::log(NetSim::Logger::Level::INFO, currentTime, NetSim::Logger::SIMULATOR, "SIM",
        "Starting discrete event simulation...");


    while (!eventQueue.empty()) {

        std::unique_ptr<Event> current = std::move(const_cast<std::unique_ptr<Event> &>(eventQueue.top()));
        eventQueue.pop(); // is this safe now?

        currentTime = current->tracker->getCurrentTime();

        NetSim::Logger::log(NetSim::Logger::Level::DEBUG, currentTime, NetSim::Logger::GENERAL, "EVT",
            "Executing: " + current->getDescription());

        this->currentTime = current->tracker->getCurrentTime(); // Update simulator's current time
        current->execute();
    }

    NetSim::Logger::log(NetSim::Logger::Level::INFO, currentTime, NetSim::Logger::SIMULATOR, "SIM",
        "Simulation completed successfully! Final time: " + std::to_string(currentTime) + "s");
}

double Simulator::getCurrentTime() const { return currentTime; }
