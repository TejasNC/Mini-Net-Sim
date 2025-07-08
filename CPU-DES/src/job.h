#ifndef JOB_H
#define JOB_H

#include "id.h"
#include "simulator.h"

#include <memory>
#include <random>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

extern double job_interarrival_time_mean, time_quantum; // bad practice, but for simplicity

class Simulator;
class Timer;
class JobArrival;
class JobCompletion;

/*
 * abstract class for a job in the cpu scheduling simulation
 * any future implementation of a job should extend this class
 */
class Job {
    private:
        static JobID next_job_id;

        std::exponential_distribution<double> job_interarrival_time_dist;

    public:
        Job(Simulator& simulator, double job_interarrival_time_mean, double job_duration)
            : job_interarrival_time_dist(1.0 / job_interarrival_time_mean),
              simulator(simulator), job_id(next_job_id++),
              job_duration(job_duration),
              job_duration_left(job_duration)
        {
        }
        virtual ~Job() = default;

        Simulator& simulator;
        JobID const job_id;
        double job_duration;
        double job_duration_left;

        virtual void process(std::shared_ptr<JobCompletion> job_completion);
        virtual void process(std::shared_ptr<JobArrival> job_arrival);
};


/*
 * timer class for job arrival
 */
class JobArrival : public Timer {
public:
    std::exponential_distribution<double> interarrival_time_dist;

    JobArrival(std::exponential_distribution<double> interarrival_time_dist)
        : interarrival_time_dist(interarrival_time_dist)
    {
    }
    void trigger(std::shared_ptr<Job> n) override;
};

/*
 * timer class for job completion
 */
class JobCompletion : public Timer {
public:
    JobCompletion()
    {
    }

    void trigger(std::shared_ptr<Job> n) override;
};


#endif // JOB_H
