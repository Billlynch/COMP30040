#ifndef RANDOMNOISECALCULATOR_H
#define RANDOMNOISECALCULATOR_H

#include <QObject>
#include <cmath>
#include <random>

/*!
 * \brief The RandomNoiseCalculator class
 * This defines all the method the noise calculator will implement
 */
class RandomNoiseCalculator : public QObject {
  Q_OBJECT
public:
  RandomNoiseCalculator(int mean, int deviation);
  // explicit RandomNoiseCalculato(QObject *parent = nullptr);

  void generate();
  void setMean(double mean);
  void setDeviation(double dev);

signals:
  void newRandomNoiseGeneration(std::normal_distribution<>, std::mt19937);

private:
  double m_mean, m_dev;
  std::random_device rd{};
  std::mt19937 *gen = new std::mt19937(rd());
};

#endif // RANDOMNOISECALCULATOR_H
