#include "simulator.h"
#include "job.h"

#include <cmath>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>

static std::ofstream logfile;
double job_interarrival_time_mean, time_quantum;
class Event;

int main(int argc, char const* argv[])
{
    /*
     * process command line arguments
     */
    double duration;
    unsigned int simulator_seed;
    if (argc != 4 && argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <job_interarrival_time_mean> <duration> <time_quantum> [simulator_seed] (for non-preemptive scheduling, time_quantum should be kept -1)\n";
        return 0;
    } else {
        job_interarrival_time_mean = std::stod(argv[1]);
        duration = std::stod(argv[2]);
        time_quantum = std::stod(argv[3]);
        if (argc == 5) {
            simulator_seed = std::stoull(argv[4]);
        } else {
            simulator_seed = std::random_device {}();
        }
    }

    /*
     * prepare output directory
     */
    std::string output_dir;
    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::stringstream ss;
        ss << std::put_time(&tm, "cpu-simulator-output_%Y-%m-%d-%H-%M-%S");
        output_dir = ss.str();
    }
    std::filesystem::create_directory(output_dir);

    /*
     * prepare logfile
     */
    logfile = std::ofstream(output_dir + "/log");

    logfile << "*** Simulator for CPU Job Scheduling ***\n";
    logfile << "*** " << output_dir << " ***\n";
    logfile << "*** [simulator_rng seed=" << simulator_seed << "] ***\n";
    logfile << "*** Invocation: ";
    for (int i = 0; i < 4; ++i)
        logfile << argv[i] << " ";
    logfile << simulator_seed << " ***\n";

    logfile.flush();

    /*
     * simulator random number generator
     */
    std::mt19937 simulator_rng(simulator_seed);

    Simulator simulator(simulator_rng, logfile);

    /*
     * create initial events
     */
    std::shared_ptr<Job> n = std::make_shared<Job>(simulator, job_interarrival_time_mean, 2);
    simulator.jobs[n->job_id] = n;
    std::shared_ptr<JobArrival> first_job_arrival = std::make_shared<JobArrival>(std::exponential_distribution<double>(1.0 / job_interarrival_time_mean));
    std::vector<std::shared_ptr<Event>> init_events;
    std::shared_ptr<Event> first_job_arrival_event = create_synthetic_timed_event(first_job_arrival, n->job_id, 0.0);
    init_events.push_back(first_job_arrival_event);

    /*
     * run simulation
     */
    simulator.init(0.0);
    simulator.inject_events(init_events);
    simulator.run(duration);

    logfile << "*** Simulation complete ***\n";
    logfile.close();

    return 0;
}

void custom_assert(bool b, char const* expr, char const* func, int line, char const* file)
{
    if (!b) {
        logfile << std::flush;
        std::cerr << "Assertion failed:\n";
        std::cerr << "\t" << expr << "\n";
        std::cerr << "in `" << func << "` at " << file << ":" << line << "\n";
        exit(1);
    }
}
