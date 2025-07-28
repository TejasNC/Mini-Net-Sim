#include "../../include/packets/packet-time-tracker.hpp"
#include "../../include/simulator.hpp"

PacketTimeTracker::PacketTimeTracker(Simulator* simulator) : sim(simulator) {
    this->currentTime = simulator->getCurrentTime();
}

void PacketTimeTracker::schedule(std::unique_ptr<Event> event) const { sim->schedule(std::move(event)); }

double PacketTimeTracker::getCurrentTime() const { return currentTime; }

double PacketTimeTracker::computeTransmissionDelay(int sizeBytes, double bandwidthMbps) const {
    return (sizeBytes * 8.0) / (bandwidthMbps * 1e6);
}

double PacketTimeTracker::computePropagationDelay(double delayMs) const { return delayMs / 1000.0; }

double PacketTimeTracker::computeTotalDelay(int sizeBytes, double bandwidthMbps, double delayMs) const {
    return computeTransmissionDelay(sizeBytes, bandwidthMbps) + computePropagationDelay(delayMs);
}
