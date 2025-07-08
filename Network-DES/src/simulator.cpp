#include "../include/simulator.hpp"
#include <iostream>

Simulator::Simulator() : eventQueue(), currentTime(0.0) {}

void Simulator::schedule(std::unique_ptr<Event> event) {
    if (event) {
        if (event->time < currentTime) {
            std::cerr << "Warning: Scheduling event in the past (event time: "
                      << event->time << ", current time: " << currentTime << ")" << std::endl;
        }
        eventQueue.push(std::move(event)); // Use move semantics to transfer ownership
    } else {
        std::cerr << "Error: Attempted to schedule a null event." << std::endl;
    }
}

void Simulator::run() {
    while (!eventQueue.empty()) {
        // Safe way to extract from priority queue
        std::unique_ptr<Event> current = std::move(const_cast<std::unique_ptr<Event>&>(eventQueue.top()));
        eventQueue.pop();

        currentTime = current->time;

        std::cout << "[Time " << currentTime << "] Executing: " << current->getDescription() << "\n";

        SimulatorContext ctx(this);
        current->execute(ctx);
    }
}

SimulatorContext::SimulatorContext(Simulator *simulator) : sim(simulator) {}

void SimulatorContext::schedule(std::unique_ptr<Event> event) const { sim->schedule(std::move(event)); }

double SimulatorContext::getCurrentTime() const { return sim->getCurrentTime(); }

double SimulatorContext::computeTransmissionDelay(int sizeBytes, double bandwidthMbps) const {
    return (sizeBytes * 8.0) / (bandwidthMbps * 1e6);
}

double SimulatorContext::computePropagationDelay(double delayMs) const { return delayMs / 1000.0; }

double SimulatorContext::computeTotalDelay(int sizeBytes, double bandwidthMbps, double delayMs) const {
    return computeTransmissionDelay(sizeBytes, bandwidthMbps) + computePropagationDelay(delayMs);
}

double Simulator::getCurrentTime() const {
    return currentTime;
}
