#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "id.h"

#include <cstddef>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <queue>
#include <random>
#include <set>

class Job;

/*
 * abstract class to be extended by any class
 * whose objects should be given in timed interval
 */
class Timer : public std::enable_shared_from_this<Timer> {
  public:
    virtual void trigger(std::shared_ptr<Job> n) = 0;
    virtual ~Timer() = default;
};

class Event {
  public:
    /*
     * Event class has two members, one for the current timestamp
     * and the other one for the job for which the event has been registered
     */
    JobID const job;
    double const time;

    Event(JobID r, double t) : job(r), time(t) {}

    virtual ~Event() = default;
    virtual void process(std::shared_ptr<Job> n) = 0;
};

class TimedEvent : public Event {
  public:
    std::shared_ptr<Timer> trigger;

    TimedEvent(JobID receiver, std::shared_ptr<Timer> trigger, double time)
        : Event(receiver, time), trigger(trigger) {}

    void process(std::shared_ptr<Job> n) override { trigger->trigger(n); }
};

/*
 * functions to create synthetic event for injection
 */
std::shared_ptr<Event>
create_synthetic_timed_event(std::shared_ptr<Timer> timer, JobID receiver,
                             double time);

struct EventComparator {
    bool operator()(std::shared_ptr<Event> e1, std::shared_ptr<Event> e2);
};

/*
 * simulator class
 */
class Simulator {
  private:
    std::ostream &logging;

    /*
     * event queue and cpu ready queue
     * event queue is a priority queue that is sorted by the time of the
     * events
     */
    std::priority_queue<std::shared_ptr<Event>,
                        std::vector<std::shared_ptr<Event>>, EventComparator>
        event_queue;
    double current_time;

  public:
    std::mt19937 &rng;

    std::map<JobID, std::shared_ptr<Job>> jobs;
    std::queue<JobID> cpu_ready_queue;
    bool cpu_busy = false;

    Simulator(std::mt19937 &rng, std::ostream &logging)
        : logging(logging), rng(rng) {}

    /*
     * util functions that can be called by CPU and Jobs
     */
    double get_time() const { return current_time; }

    void log(JobID requester, std::string log);

    void register_timer(JobID receiver, std::shared_ptr<Timer> trigger,
                        double time_delay);

    std::shared_ptr<Job> get_job() {
        JobID job_id = cpu_ready_queue.front();
        cpu_ready_queue.pop();
        return jobs.at(job_id);
    }

    void add_to_cpu_ready_queue(JobID job_id) { cpu_ready_queue.push(job_id); }

    /*
     * functions for managing the simulation
     */
    void init(double time);
    void inject_events(std::vector<std::shared_ptr<Event>> events);
    void run(double duration);
};

#endif // SIMULATOR_H
