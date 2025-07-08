# CPU Job Scheduling Simulator
## Compile
```
make -j
```
## Run
```
cpu-simulator <job_interarrival_time_mean> <duration> <time_quantum> [simulator_seed]
```
where
 - `job_interarrival_time_mean` is the mean job interarrival time (in `ms`) *for each job* when sampling from an exponential distribution
 - `duration` is how long (in `ms` in terms of in-simulation time) the simulation should be run for
 - `time_quantum` is the duration of a time slice in the scheduling policy, for non-preemptive scheduling keep the value as -1
 - `simulator_seed` is a seed for random events *during* simulation such as time between arrival of two jobs and duration of a job
