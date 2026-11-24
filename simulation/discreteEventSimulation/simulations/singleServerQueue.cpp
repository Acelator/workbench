#include "../simulator.h"

#include <vector>
#include <limits>

// Based on the simulation described in Simulation - Ross S.M (2013)
// Chapter 7.2 -> A single server queueing system

// Potency function that defines the nonhomogeneous Poisson process.
double potency(double t)
{
    return 2;
}

double bound = 4;

// Parameters
double T{10}; // Time when no more arrivals are allowed.

// std::function<double(double, double)> yDistribution();
// yDistribution = exponential_pdf;

auto yDistribution = [](double rate) -> double
{ return -1. / rate * std::log(generate()); };

// Naive implementation
void simulationNaive()
{
    // Time variable
    double t{0}; // Elapsed time of the simulation

    // Counter variables
    int Na{0}; // Number of arrivals by time t
    int Nd{0}; // Number of departures by time t

    // System state variable
    int n{0}; // Number of customers in the system

    // Observed variables
    std::vector<double> arrivals{};   // Arrival time of customer i
    std::vector<double> departures{}; // Departure time of customer i
    double Tp{0};                     // Time last customer departures.

    double ta{timeOfNextArrival(t, potency, bound)};    // Time of next arrival
    double td{std::numeric_limits<double>::infinity()}; // Time of next departure (infty, if there isnt any customer in the system)

    // Starts the simulation
    while (n > 0 || t < T)
    {
        if (ta <= td && ta <= T)
        {
            t = ta;
            Na += 1;
            n += 1;

            ta = timeOfNextArrival(t, potency, bound); // Time of the next arrival

            if (ta < T)
                std::cout << "Time of next arrival: " << ta << std::endl;

            if (n == 1)
            {
                td = t + yDistribution(bound);
            }

            arrivals.push_back(t);
        }

        if (td < ta && td <= T)
        {
            t = td;
            n = n - 1;
            Nd += 1;
            if (n == 0)
                td = std::numeric_limits<double>::infinity();
            else
                td = t + yDistribution(bound);

            departures.push_back(t);
        }

        if (std::min(ta, td) > T && n > 0)
        {
            t = td;
            n = n - 1;
            Nd += 1;
            if (n == 0)
                td = std::numeric_limits<double>::infinity();
            else
                td = t + yDistribution(bound);

            departures.push_back(t);
        }

        if (std::min(ta, td) > T && n == 0)
        {
            Tp = std::max(0., t - T);
            break;
        }
    }

    // Logging
    std::cout << "Time last customer departs after no more people are allowed in: " << Tp << std::endl;
    std::cout << "Number of arrivals: " << Na << std::endl;
    std::cout << "Number of departures: " << Nd << std::endl;
}

// Event driven simulation of the single server queue
void eventDrivenSimulation()
{
    Simulator sim{};
    std::function<void()> arrival;
    std::function<void()> departure;

    // Time variable (Owned by the simulator)

    // Counter variables
    int Na{0}; // Number of arrivals by time t
    int Nd{0}; // Number of departures by time t

    // System state variable
    int n{0}; // Number of customers in the system

    // Observed variables
    std::vector<double> arrivals{};   // Arrival time of customer i
    std::vector<double> departures{}; // Departure time of customer i
    double Tp{0};                     // Time last customer departures.

    double ta{timeOfNextArrival(sim.now(), potency, bound)}; // Time of next arrival
    double td{std::numeric_limits<double>::infinity()};      // Time of next departure (infty, if there isnt any customer in the system)

    arrival = [&]()
    {
        Na += 1;
        n += 1;

        ta = timeOfNextArrival(sim.now(), potency, bound); // Time of the next arrival

        if (ta <= T)
        {
            // std::cout << "Time of next arrival: " << ta << std::endl;
            sim.schedule(ta - sim.now(), arrival);
        }

        if (n == 1)
        {
            td = sim.now() + yDistribution(bound);
            sim.schedule(td - sim.now(), departure);
        }

        arrivals.push_back(sim.now());
    };

    departure = [&]()
    {
        n = n - 1;
        Nd += 1;
        if (n == 0)
            td = std::numeric_limits<double>::infinity();
        else
        {
            td = sim.now() + yDistribution(bound);
            sim.schedule(td - sim.now(), departure);
        }

        departures.push_back(sim.now());
    };

    if (ta <= T)
        sim.schedule(ta, arrival);

    sim.run(std::numeric_limits<double>::infinity());
    Tp = std::max(0., sim.now() - T);

    // Logging
    std::cout << "Time last customer departs after no more people are allowed in: " << Tp << std::endl;
    std::cout << "Number of arrivals: " << Na << std::endl;
    std::cout << "Number of departures: " << Nd << std::endl;
}

int main()
{
    // simulationNaive();

    eventDrivenSimulation();
}