#include "simulator.h"

double timeOfNextArrival(double s, std::function<double(double)> pot, double bound)
{
    // Disallow posible infinite loop
    while (true)
    {
        double U{generate()};

        s += -1. / bound * std::log(U);

        U = generate();

        if (U <= pot(s) / bound)
            return s;
    }
}

void Simulator::run(double end_time)
{
    while (!event_list.empty() && event_list.top().time <= end_time)
    {
        Event e = event_list.top();
        event_list.pop();
        clock = e.time; // Jump the simulation clock
        e.action();     // Execute the event routine
    }
}

void Simulator::schedule(double delay, std::function<void()> action)
{
    event_list.push({clock + delay, std::move(action)});
}
