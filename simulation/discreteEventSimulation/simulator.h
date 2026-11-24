#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <math.h>

#include "../distributions.h"

// TODO: Allow to import the simulation of the RV from here
inline double generate()
{
    return ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);
}

double timeOfNextArrival(double s, std::function<double(double)> pot, double bound);

struct Event
{
    double time;
    std::function<void()> action;

    // Min-heap comparator: earliest event time comes first
    bool operator>(const Event &other) const
    {
        return time > other.time;
    }
};

// Queueing model with the customers arriving in accordance with a nonhomogeneous Poisson process with a given bounded intensity function.
class Simulator
{
    double clock = 0.0;
    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> event_list;

public:
    double now() const { return clock; }
    void schedule(double delay, std::function<void()> action);
    void run(double end_time);
};