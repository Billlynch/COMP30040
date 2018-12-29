#include "collideableobject.h"

bool CollideableObject::interceptPlane(Ray& ray, double& t) {
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

CollideableObject::CollideableObject(const Eigen::Vector3d& location, int side, const Eigen::Vector3d& normal) :
  m_location(location), side(side), m_normal(normal){
}

CollideableObject::~CollideableObject() = default;

const Eigen::Vector3d& CollideableObject::getLocation() const {
  return CollideableObject::m_location;
}

const Eigen::Vector3d& CollideableObject::getNormal() const {
  return CollideableObject::m_normal;
}

void CollideableObject::setLocation(const Eigen::Vector3d& location) {
    CollideableObject::m_location = location;
}

Eigen::Vector3d CollideableObject::newPosition(Eigen::Vector3d samplePositition, double angle, Eigen::Vector3d rayDirection)
{
    double adjLength = samplePositition(1) - this->m_location(2);
    double oppLength = std::tan(angle * (M_PI / 180.0)) * adjLength;
    this->m_location(0) = oppLength * side;

    if  (side != 0) {
        this->m_normal = rayDirection;
        this->m_normal(0) = this->m_normal(0) * side;
    }
    return this->m_location;
}
