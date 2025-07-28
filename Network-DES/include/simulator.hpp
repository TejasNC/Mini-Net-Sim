#pragma once

#include "event.hpp"
#include "packets/packet-time-tracker.hpp"
#include <memory>
#include <queue>
#include <vector>

class Event;

class Simulator {
  private:
    struct EventComparator {
        bool operator()(const std::unique_ptr<Event> &a, const std::unique_ptr<Event> &b) const {
            return a->tracker->getCurrentTime() > b->tracker->getCurrentTime(); // Min-heap based on event time
        }
    };

    std::priority_queue<std::unique_ptr<Event>, std::vector<std::unique_ptr<Event>>, EventComparator> eventQueue;
    double currentTime; // TODO: check if this is needed

  public:
    Simulator();

    void schedule(std::unique_ptr<Event> event);
    void run();

    double getCurrentTime() const; // TODO: check if this is needed
};
