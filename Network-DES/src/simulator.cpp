#include "../include/simulator.hpp"
#include "../include/packets/packet-time-tracker.hpp"
#include <iostream>

Simulator::Simulator() : eventQueue(), currentTime(0.0) {}

void Simulator::schedule(std::unique_ptr<Event> event) {

    if (event) {
        if (event->tracker->getCurrentTime() < currentTime) {
            std::cerr << "Warning: Scheduling event in the past     (event time: " << event->tracker->getCurrentTime()
                      << ", current time: " << currentTime << ")" << std::endl;
        }
        eventQueue.push(std::move(event)); // Use move semantics to transfer ownership
    } else {
        std::cerr << "Error: Attempted to schedule a null event." << std::endl;
    }
}

void Simulator::run() {

    while (!eventQueue.empty()) {

        std::unique_ptr<Event> current = std::move(const_cast<std::unique_ptr<Event> &>(eventQueue.top()));
        eventQueue.pop(); // is this safe now?

        currentTime = current->tracker->getCurrentTime();

        std::cout << "[Time " << currentTime << "] Executing: " << current->getDescription() << "\n";

        this->currentTime = current->tracker->getCurrentTime(); // Update simulator's current time

        current->execute();
    }

    std::cout << "Simulation complete. Final time: " << currentTime << std::endl;
}

double Simulator::getCurrentTime() const { return currentTime; }
