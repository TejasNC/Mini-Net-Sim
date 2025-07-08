#include "assert.h"
#include "job.h"
#include "simulator.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>


std::shared_ptr<Event> create_synthetic_timed_event(std::shared_ptr<Timer> timer, JobID receiver, double time)
{
    return std::make_shared<TimedEvent>(receiver, timer, time);
}

bool EventComparator::operator()(std::shared_ptr<Event> e1, std::shared_ptr<Event> e2)
{
    return e1->time > e2->time;
}

void Simulator::register_timer(JobID receiver, std::shared_ptr<Timer> trigger, double time_delay)
{
    assert(time_delay >= 0.0);
    std::shared_ptr<Event> e = std::make_shared<TimedEvent>(receiver, trigger, current_time + time_delay);
    event_queue.push(e);
}

void Simulator::log(JobID requester, std::string log)
{
    logging << "[" << std::setprecision(3) << std::setw(13) << std::fixed << current_time << "ms] " << std::setw(3) << requester << ": " << log << '\n';
    logging.flush();
}

void Simulator::init(double time)
{
    event_queue = std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EventComparator> {};
    current_time = time;
}

void Simulator::inject_events(std::vector<std::shared_ptr<Event>> events)
{
    for (auto e : events)
        event_queue.push(e);
}

void Simulator::run(double duration)
{
    double end_time = current_time + duration;

    std::shared_ptr<Event> e;
    while (true) {
        e = event_queue.top();
        event_queue.pop();
        assert(e->time >= current_time);
        if (e->time > end_time) {
            event_queue.push(e);
            break;
        }
        current_time = e->time;
        e->process(jobs.at(e->job));
    }
}