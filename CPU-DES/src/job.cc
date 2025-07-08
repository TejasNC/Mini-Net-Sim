#include "job.h"

JobID Job::next_job_id = 0;

void JobArrival::trigger(std::shared_ptr<Job> n)
{
    auto t = std::static_pointer_cast<JobArrival>(shared_from_this());
    n->process(t);
}

void JobCompletion::trigger(std::shared_ptr<Job> n)
{
    n->process(std::static_pointer_cast<JobCompletion>(shared_from_this()));
}

void Job::process(std::shared_ptr<JobCompletion> job_completion)
{
    if (job_duration_left == 0) {
        simulator.log(job_id, "Job completed");
    } else {
        simulator.log(job_id, "Job interrupted because of preemptive scheduling");
        simulator.add_to_cpu_ready_queue(job_id);
    }

    if (simulator.cpu_ready_queue.empty()) {
        simulator.cpu_busy = false;
        return;
    }

    std::shared_ptr<Job> next_job = simulator.get_job();
    if (time_quantum == -1) {
        simulator.log(next_job->job_id, "Job being scheduled now");
        simulator.register_timer(next_job->job_id, job_completion, next_job->job_duration_left);
        next_job->job_duration_left = 0;
    } else {
        if (next_job->job_duration_left <= time_quantum) {
            simulator.log(next_job->job_id, "Job being scheduled now");
            simulator.register_timer(next_job->job_id, job_completion, next_job->job_duration_left);
            next_job->job_duration_left = 0;
        } else {
            simulator.log(next_job->job_id, "Job being scheduled now");
            simulator.register_timer(next_job->job_id, job_completion, time_quantum);
            next_job->job_duration_left -= time_quantum;
        }
    }
}

void Job::process(std::shared_ptr<JobArrival> job_arrival)
{
    simulator.log(job_id, "Job Arrived");
    if (!simulator.cpu_ready_queue.empty() || simulator.cpu_busy) {
        simulator.add_to_cpu_ready_queue(job_id);
    } else {
        simulator.cpu_busy = true;
        std::shared_ptr<JobCompletion> job_completion = std::make_shared<JobCompletion>();
        if (time_quantum == -1) {
            simulator.log(job_id, "Job being scheduled now");
            simulator.register_timer(job_id, job_completion, job_duration_left);
            job_duration_left = 0;
        } else {
            if (job_duration_left <= time_quantum) {
                simulator.log(job_id, "Job being scheduled now");
                simulator.register_timer(job_id, job_completion, job_duration_left);
                job_duration_left = 0;
            } else {
                simulator.log(job_id, "Job being scheduled now");
                simulator.register_timer(job_id, job_completion, time_quantum);
                job_duration_left -= time_quantum;
            }
        }
    }
    double job_interarrival_time = job_interarrival_time_dist(simulator.rng);
    std::shared_ptr<Job> next_job_arrival = std::make_shared<Job>(simulator, job_interarrival_time_mean, 5);
    simulator.jobs[next_job_arrival->job_id] = next_job_arrival;
    simulator.register_timer(next_job_arrival->job_id, job_arrival, job_interarrival_time);
}
