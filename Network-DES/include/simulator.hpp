#pragma once

#include "event.hpp"
#include <memory>
#include <queue>
#include <vector>

class Event;

class SimulatorContext {
  // Add global handles (e.g., list of nodes, log stream) later
  private:
    class Simulator *sim; // backref to Simulator
  public:
    explicit SimulatorContext(Simulator *simulator);
    void schedule(std::unique_ptr<Event> event) const;
    double getCurrentTime() const;

    // Optional delay utility
    double computeTransmissionDelay(int sizeBytes, double bandwidthMbps) const;
    double computePropagationDelay(double delayMs) const;
    double computeTotalDelay(int sizeBytes, double bandwidthMbps, double delayMs) const;
};

class Simulator {
  private:
    struct EventComparator {
        bool operator()(const std::unique_ptr<Event>& a, const std::unique_ptr<Event>& b) const {
            return a->time > b->time; // Min-heap: earlier events have higher priority
        }
    };

    std::priority_queue<std::unique_ptr<Event>, std::vector<std::unique_ptr<Event>>, EventComparator> eventQueue;
    double currentTime;

  public:
    Simulator();

    void schedule(std::unique_ptr<Event> event);
    void run();

    double getCurrentTime() const;

};
