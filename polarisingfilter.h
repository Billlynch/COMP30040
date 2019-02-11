#ifndef POLARISINGFILTER_H
#define POLARISINGFILTER_H

#include "collideableobject.h"
#include <random>

typedef Eigen::Matrix<std::complex<double>, 2, 2> Matrix22d;

// const float degreeMulitplier = static_cast<float>(180.0 / M_PI);

class PolarisingFilter : public CollideableObject {
  Q_OBJECT
  Matrix22d m_polarizationMatrix;
  Eigen::Vector2d m_targetPolarisation;
  double m_radius;
  std::complex<double> m_n1;
  std::normal_distribution<> dist;
  std::mt19937 noise_gen;
  int noiseState = 0;

public:
  PolarisingFilter(Eigen::Vector3d location, int side, Eigen::Vector3d normal,
                   double radius, std::complex<double> n1,
                   Eigen::Vector2d targetPolarisation);

  ~PolarisingFilter() {}

  void collide(Ray &ray, Eigen::Vector3d &pointOfInterception);
  bool intersect(Ray &ray, Eigen::Vector3d &pointOfInterception);
  int getType();

  Matrix22d getPolarisationMatrix();
  void newNoise(std::normal_distribution<> d, std::mt19937 gen);
  void setNoiseState(int state);
  void setTarget(Eigen::Vector2d target);

protected:
  void calculatePolarisationMatrix();

  void calculateAngleOfRefraction(std::complex<double> theta0,
                                  std::complex<double> &theta1) {
    std::complex<double> numerator1 = m_n0 * sin(theta0);
    theta1 = asin(numerator1 / m_n1); // the angle of refraction
  }

  void calculateAngleOfInterception(Ray &ray, std::complex<double> &theta0) {
    auto normal = this->getNormal();
    normal.normalize();
    Eigen::Vector3d rayDirection = ray.getDirection();
    rayDirection.normalize();
    std::complex<double> numerator0 = rayDirection.dot(normal);
    std::complex<double> denominator0 = rayDirection.norm() * normal.norm();
    theta0 = acos(numerator0 / denominator0); // the angle of incidence
  }

signals:
  void outputPolarisationUpdated(Matrix4cd polarisation);
};

#endif // POLARISINGFILTER_H
