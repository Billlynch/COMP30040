#include <utility>

#include "polarisingfilter.h"
#include <iostream>

PolarisingFilter::PolarisingFilter(Eigen::Vector3d location, int side,
                                   Eigen::Vector3d normal, double radius,
                                   std::complex<double> n1,
                                   Eigen::Vector2d targetPolarisation)
    : CollideableObject(std::move(location), side, std::move(normal)) {
  m_radius = radius;
  m_n1 = n1;
  m_targetPolarisation = std::move(targetPolarisation);
  calculatePolarisationMatrix();
}

void PolarisingFilter::collide(Ray &ray, Eigen::Vector3d &pointOfInterception) {
    // if the Er is different emit it from here.
    auto tempEr = Eigen::Vector2cd(ray.getPolarisation()(0,0), ray.getPolarisation()(1,1));
    if (tempEr != this->Er) {
        this->Er = tempEr;
        emit outputEr(Er);
    }


  this->calculatePolarisationMatrix();
  ray.setPolarisation(m_polarizationMatrix * ray.getPolarisation());
  ray.calculationMatrixMultiplication(m_polarizationMatrix);

  // assuming no change in direction
  ray.setOrigin(pointOfInterception +
                ray.getDirection() * 0.01f); // move it along the normal so it
                                             // won't hit the same object again
  emit outputPolarisationUpdated(ray.getPolarisation());
}

bool PolarisingFilter::intersect(Ray &ray,
                                 Eigen::Vector3d &pointOfInterception) {
  double t;

  if (this->interceptPlane(ray, t)) {
    pointOfInterception = ray.getOrigin() + ray.getDirection() * t;
    Eigen::Vector3d v = pointOfInterception - this->getLocation();
    double d2 = v.dot(v);

    return (sqrt(d2) <= m_radius);
  }

  return false;
}

int PolarisingFilter::getType() { return 2; }

void PolarisingFilter::calculatePolarisationMatrix() {
  Eigen::Vector2d down = Eigen::Vector2d(0.0, -1.0);
  m_targetPolarisation.norm();
  down.norm();
  auto temp1 = down.dot(m_targetPolarisation);
  auto temp2 = down.norm() * m_targetPolarisation.norm();
  std::complex<double> a = acos(temp1 / temp2);

  double angle = a.real();

  m_polarizationMatrix(0, 0) = pow(cos(angle), 2.0);
  m_polarizationMatrix(0, 1) = sin(angle) * cos(angle);
  m_polarizationMatrix(1, 0) = sin(angle) * cos(angle);
  m_polarizationMatrix(1, 1) = pow(sin(angle), 2.0);
}

Matrix22d PolarisingFilter::getPolarisationMatrix() {
  this->calculatePolarisationMatrix();
  return m_polarizationMatrix;
}


void PolarisingFilter::setTarget(Eigen::Vector2d target)
{
    this->m_targetPolarisation = target;
}
