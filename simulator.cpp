#include "simulator.h"
#include <iostream>

Simulator::Simulator()
{

}

Simulator::~Simulator() {

}

void Simulator::runSimulation(double Q, double refractiveIndex) {
    std::cout << "this is run in the sim class now with values: " << Q << refractiveIndex << std::endl;
}
