#include "collideableobject.h"

bool CollideableObject::interceptPlane(Ray &ray, Eigen::Vector3f &normal, float &t) {
    float denom = ray.getDirection().dot(normal);

    if (denom < static_cast<float>(1e-6))
    {
        Eigen::Vector3f numerator = this->getLocation() - ray.getOrigin();
        t = numerator.dot(normal) / denom;
        return t >= 0;
    }

    return false;
}

CollideableObject::CollideableObject(const Eigen::Vector3f &location) :
    m_location(location){
}

CollideableObject::~CollideableObject() {}

const Eigen::Vector3f &CollideableObject::getLocation() const {
    return CollideableObject::m_location;
}

void CollideableObject::setLocation(const Eigen::Vector3f &location) {
    CollideableObject::m_location = location;
}
