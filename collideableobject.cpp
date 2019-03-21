#include "collideableobject.h"



/*!
 * \class CollideableObject
 *
 * \brief The CollideableObject class is the base class which must be extended by
 * all objects which we want to collide in the ray tracing simulation. It enforces
 * methods which are required by the algorithm.
 *
 * \since v1
 */


/*!
 * \fn CollideableObject::interceptPlane(Ray &ray, double &t)
 *
 * Returns true if the ray intercepts with the plane which is this object.
 * output t is the distance along the ray at which the collision occurs.
 */

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


/*!
 * \brief CollideableObject::newPosition
 * \param samplePositition - The position for the sample object (works like a pivot point)
 * \param angle - The desired angle of incidence against the plane of the object
 * \param emissionDirection - The vector direction from the laser to the sample
 *
 * This uses trigonometry to calculate the desired position for the object.
 * This will be flipped for the other side of the sample (post interception with sample),
 * as for now we are just dealing with perfect reflection.
 */

void CollideableObject::newPosition(Eigen::Vector3d samplePositition, double angle,
                               Eigen::Vector3d emissionDirection) {
  double adjLength = samplePositition(1) - this->m_location(1);
  double oppLength = std::tan(angle * (M_PI / 180.0)) * adjLength;
  this->m_location(0) = oppLength * side;

  if (side != 0) {
    this->m_normal = std::move(emissionDirection);
    this->m_normal(0) = this->m_normal(0) * side;
  }
}

/*!
 * \brief CollideableObject::setEnabled
 * \param state
 *
 * enabled or disables the object in the 3D visualisation and in the ray tracing algorithm it
 * will turn off the collision detection.
 */
void CollideableObject::setEnabled(int state) {
  this->collisionsEnabled = state;
}
