#include "collideableobject.h"

bool CollideableObject::interceptPlane(Ray& ray, Eigen::Vector3d& normal, double& t) {
  Eigen::Vector3d rayDirection = ray.getDirection();
  normal.normalize();

  double denom = normal.dot(rayDirection);

  if (fabs(denom) > 0.0001) {
    Eigen::Vector3d numerator = this->getLocation() - ray.getOrigin();
    t = numerator.dot(normal) / denom;
    return t >= 0;
  }

  return false;
}

CollideableObject::CollideableObject(const Eigen::Vector3d& location, int side) :
  m_location(location), side(side){
}

CollideableObject::~CollideableObject() = default;

const Eigen::Vector3d& CollideableObject::getLocation() const {
  return CollideableObject::m_location;
}

void CollideableObject::setLocation(const Eigen::Vector3d& location) {
    CollideableObject::m_location = location;
}

Eigen::Vector3d CollideableObject::newPosition(Eigen::Vector3d samplePositition, double angle)
{
    double adjLength = samplePositition(1) - this->m_location(2);
    double oppLength = std::tan(angle * (M_PI / 180.0)) * adjLength;
    this->m_location(0) = oppLength * side;
    return this->m_location;
}
