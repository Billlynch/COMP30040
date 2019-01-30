#ifndef PEM_H
#define PEM_H

#include "collideableobject.h"
#include <QTimer>
#include <random>

typedef Eigen::Matrix<std::complex<double>, 2, 2> Matrix22cd;

class PEM :  public CollideableObject {
  Q_OBJECT
 private:
  Matrix22cd m_polarizationMatrix;
  std::complex<double> m_phaseAmplitude;
  std::complex<double> m_angularFrequency;
  double time, m_radius;
  std::complex<double> m_i = {0, 1};
  int noiseState = 0;
  std::normal_distribution<> dist;
  std::mt19937 noise_gen;

  void calculatePolarisationMatrix();

 public:
  PEM(Eigen::Vector3d location,
      int side,
      Eigen::Vector3d normal,
      double radius,
      std::complex<double> phaseAmplitude,
      std::complex<double> angularFrequency);

  ~PEM() {}
  int getType();

  void collide(Ray& ray, Eigen::Vector3d& pointOfInterception);
  bool intersect(Ray& ray, Eigen::Vector3d& pointOfInterception);
  void incrementTime();

  void setNoiseState(int state);
  void newNoise(std::normal_distribution<> d, std::mt19937 gen);

 signals:
  void outputPolarisationUpdated(Matrix4cd polarisation);
};

#endif // PEM_H
