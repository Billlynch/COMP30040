#include "randomnoisecalculator.h"

RandomNoiseCalculator::RandomNoiseCalculator(int mean, int deviation) {
  this->m_mean = static_cast<double>(mean);
  this->m_dev = static_cast<double>(deviation);
}

void RandomNoiseCalculator::generate() {
  std::normal_distribution<> d{m_mean, m_dev};
  emit newRandomNoiseGeneration(d, *gen);
}

void RandomNoiseCalculator::setMean(double mean) {
  this->m_mean = mean;
  this->generate();
}

void RandomNoiseCalculator::setDeviation(double dev) {
  this->m_dev = dev;
  this->generate();
}
