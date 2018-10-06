#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <complex>

class Simulator
{
public:
    Simulator();
    ~Simulator();

    void runSimulation(float Q, std::complex<float> refractiveIndex);
};

#endif // SIMULATOR_H
