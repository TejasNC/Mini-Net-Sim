#pragma once

#include <memory>

// Forward declarations to avoid circular dependency
class Simulator;
class Event;

class PacketTimeTracker {
    // Add global handles (e.g., list of nodes, log stream) later
  private:
    Simulator* sim; // backref to Simulator

  public:
    double currentTime; // Current simulation time for this packet
    PacketTimeTracker(Simulator* simulator);
    void schedule(std::unique_ptr<Event> event) const;

    double getCurrentTime() const;

    // delay utilities, TODO:  moving these to respective nodes
    double computeTransmissionDelay(int sizeBytes, double bandwidthMbps) const;
    double computePropagationDelay(double delayMs) const;
    double computeTotalDelay(int sizeBytes, double bandwidthMbps, double delayMs) const;

};
