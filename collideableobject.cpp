#include <utility>

#include <utility>

#include <utility>

#include "collideableobject.h"
#include <iostream>

bool CollideableObject::interceptPlane(Ray &ray, double &t) {

  if (this->collisionsEnabled == 0) {
    return false;
  }

  Eigen::Vector3d rayDirection = ray.getDirection();
  m_normal.normalize();

  double denom = m_normal.dot(rayDirection);

  if (fabs(denom) > 0.0001) {
    Eigen::Vector3d numerator = this->getLocation() - ray.getOrigin();
    t = numerator.dot(m_normal) / denom;
    return t >= 0;
  }

  return false;
}

CollideableObject::CollideableObject(Eigen::Vector3d location, int side,
                                     Eigen::Vector3d normal)
    : m_location(std::move(location)), side(side), m_normal(std::move(normal)) {
}

CollideableObject::~CollideableObject() = default;

const Eigen::Vector3d &CollideableObject::getLocation() const {
  return CollideableObject::m_location;
}

const Eigen::Vector3d &CollideableObject::getNormal() const {
  return CollideableObject::m_normal;
}

void CollideableObject::setLocation(const Eigen::Vector3d &location) {
  CollideableObject::m_location = location;
}

Eigen::Vector3d *
CollideableObject::newPosition(Eigen::Vector3d samplePositition, double angle,
                               Eigen::Vector3d emissionDirection) {
  double adjLength = samplePositition(1) - this->m_location(1);
  double oppLength = std::tan(angle * (M_PI / 180.0)) * adjLength;
  this->m_location(0) = oppLength * side;

  if (side != 0) {
    this->m_normal = std::move(emissionDirection);
    this->m_normal(0) = this->m_normal(0) * side;
  }
  return nullptr;
}

void CollideableObject::setEnabled(int state) {
  this->collisionsEnabled = state;
}
