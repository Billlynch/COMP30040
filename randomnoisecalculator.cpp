#include "randomnoisecalculator.h"

/*!
 * \brief RandomNoiseCalculator::RandomNoiseCalculator
 * \param mean
 * \param deviation
 * Constructs the object with the supplied generator and devation.
 */
RandomNoiseCalculator::RandomNoiseCalculator(int mean, int deviation) {
  this->m_mean = static_cast<double>(mean);
  this->m_dev = static_cast<double>(deviation);
}

/*!
 * \brief RandomNoiseCalculator::generate
 * This is called whenever the mean or the deviation is changed and emits
 * to the object it is tied to the new generator and distrobution.
 */
void RandomNoiseCalculator::generate() {
  std::normal_distribution<> d{m_mean, m_dev};
  emit newRandomNoiseGeneration(d, *gen);
}

/*!
 * \brief RandomNoiseCalculator::setMean
 * \param mean
 * On change of the mean in the UI the random is updated and generate is called
 */
void RandomNoiseCalculator::setMean(double mean) {
  this->m_mean = mean;
  this->generate();
}

/*!
 * \brief RandomNoiseCalculator::setMean
 * \param mean
 * On change of the deviation in the UI the random is updated and generate is called
 */
void RandomNoiseCalculator::setDeviation(double dev) {
  this->m_dev = dev;
  this->generate();
}
