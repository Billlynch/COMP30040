#include "polarisingfilter.h"
#include <iostream>


PolarisingFilter::PolarisingFilter(Eigen::Vector3d location,
                                   int side,
                                   Eigen::Vector3d normal,
                                   double radius,
                                   std::complex<double> n1,
                                   Eigen::Vector2d targetPolarisation) : CollideableObject (location, side) {
  m_normal = normal;
  m_radius = radius;
  m_n1 = n1;
  m_targetPolarisation = targetPolarisation;
  calculatePolarisationMatrix();
}

void PolarisingFilter::collide(Ray& ray, Eigen::Vector3d& pointOfInterception) {
  ray.setPolarisation(m_polarizationMatrix * ray.getPolarisation());
  ray.calculationMatrixMultiplication(m_polarizationMatrix);

  // assuming no change in direction
  ray.setOrigin(pointOfInterception + ray.getDirection() * 0.01f); // move it along the normal so it won't hit the same object again
  emit outputPolarisationUpdated(ray.getPolarisation());
}

bool PolarisingFilter::intersect(Ray& ray, Eigen::Vector3d& pointOfInterception) {
  double t;

  if (this->interceptPlane(ray, m_normal, t)) {
    pointOfInterception = ray.getOrigin() + ray.getDirection() * t;
    Eigen::Vector3d v = pointOfInterception - this->getLocation();
    double d2 = v.dot(v);
    if (sqrt(d2) <= m_radius) {
      //std::cout << "collide with polariser" << std::endl;
    }
    return (sqrt(d2) <= m_radius);
  }

  return false;
}

int PolarisingFilter::getType()
{
  return 2;
}

void PolarisingFilter::calculatePolarisationMatrix() {
  Eigen::Vector2d right = Eigen::Vector2d(1.0, 0.0);
  m_targetPolarisation.norm();
  right.norm();
  auto temp1 = right.dot(m_targetPolarisation);
  auto temp2 = right.norm() * m_targetPolarisation.norm();
  std::complex<double> a = acos(temp1 / temp2);

  double angle = a.real();

  m_polarizationMatrix(0, 0) = pow(cos(angle), 2.0);
  m_polarizationMatrix(0, 1) = sin(angle) * cos(angle);
  m_polarizationMatrix(1, 0) = sin(angle) * cos(angle);
  m_polarizationMatrix(1, 1) = pow(sin(angle), 2.0);
}
