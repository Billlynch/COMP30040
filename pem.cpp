#include "pem.h"

#include <iostream>


void PEM::calculatePolarisationMatrix() {
  std::complex<double> phi =  m_phaseAmplitude * sin(m_angularFrequency * time);

  std::complex<double> a =  exp((m_i * phi ) / 2.0);
  std::complex<double> b =  exp((-m_i * phi ) / 2.0);

  m_polarizationMatrix << a, 0.0, 0.0, b;
}

PEM::PEM(Eigen::Vector3d location,
         int side,
         Eigen::Vector3d normal,
         double radius,
         std::complex<double> phaseAmplitude,
         std::complex<double> angularFrequency) : CollideableObject (location, side),
  m_phaseAmplitude(phaseAmplitude),
  m_angularFrequency(angularFrequency),
  m_normal(normal),
  m_radius(radius)
{}

int PEM::getType()
{
    return 1;
}

bool PEM::intersect(Ray& ray, Eigen::Vector3d& pointOfInterception) {
  double t;

  if (this->interceptPlane(ray, m_normal, t)) {
    pointOfInterception = ray.getOrigin() + ray.getDirection() * t;
    Eigen::Vector3d v = pointOfInterception - this->getLocation();
    double d2 = v.dot(v);
    if (sqrt(d2) <= m_radius) {
      //std::cout << "collide with PEM" << std::endl;
    }
    return (sqrt(d2) <= m_radius);
  }

  return false;
}

void PEM::incrementTime() {
  this->time++;
}


void PEM::collide(Ray& ray, Eigen::Vector3d& pointOfInterception) {
  calculatePolarisationMatrix();

  ray.setPolarisation(m_polarizationMatrix * ray.getPolarisation());
  ray.calculationMatrixMultiplication(m_polarizationMatrix);

  // assuming no change in direction
  ray.setOrigin(pointOfInterception + ray.getDirection() * 0.01f); // move it along the normal so it won't hit the same object again

  emit outputPolarisationUpdated(ray.getPolarisation());
}


