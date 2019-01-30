#include <utility>

#include <utility>

#include <utility>

#include <utility>

#include <utility>

#include <utility>

#include <utility>

#include "ray.h"


Ray::Ray(Eigen::Vector3d origin, Eigen::Vector3d direction, Matrix4cd polarisation, Eigen::Vector2d targetPolarisation):
  m_origin(std::move(origin)),
  m_direction(std::move(direction)),
  m_polarisation(std::move(polarisation)),
  m_targetPolarisation(std::move(targetPolarisation)) {

  Eigen::Vector2d right = Eigen::Vector2d(1.0, 0.0);
  m_targetPolarisation.norm();
  right.norm();
  auto temp1 = right.dot(m_targetPolarisation);
  auto temp2 = right.norm() * m_targetPolarisation.norm();
  std::complex<double> a = acos(temp1 / temp2);

  double angle = a.real();

  m_calculationMatrix(0, 0) = pow(cos(angle), 2.0);
  m_calculationMatrix(0, 1) = sin(angle) * cos(angle);
  m_calculationMatrix(1, 0) = sin(angle) * cos(angle);
  m_calculationMatrix(1, 1) = pow(sin(angle), 2.0);
}

Ray::~Ray() = default;

Eigen::Vector3d Ray::getOrigin() {
  return Ray::m_origin;
}

Eigen::Vector3d Ray::getDirection() {
  return Ray::m_direction;
}

Matrix4cd Ray::getPolarisation() {
  return Ray::m_polarisation;
}

Matrix4cd Ray::getCalculationMatrix() {
  return m_calculationMatrix;
}


void Ray::setOrigin(Eigen::Vector3d origin) {
  Ray::m_origin = std::move(origin);
}

void Ray::setDirection(Eigen::Vector3d direction) {
  Ray::m_direction = std::move(direction);
}

void Ray::setPolarisation(Matrix4cd polarisation) {
  Ray::m_polarisation = std::move(polarisation);
}


void Ray::calculationMatrixMultiplication(Matrix4cd& x) {
  m_calculationMatrix = m_calculationMatrix * x;
}

