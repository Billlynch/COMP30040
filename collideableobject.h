#ifndef COLLIDEABLEOBJECT_H
#define COLLIDEABLEOBJECT_H
#include "ray.h"

#include <Core>


class CollideableObject
{
    Eigen::Vector3f location;

protected:
    bool interceptPlane(Ray &ray, Eigen::Vector3f &normal, float &t) {
            float denom = ray.getDirection().dot(normal);

            if (denom < 1e-6)
            {
                Eigen::Vector3f numerator = this->getLocation() - ray.getOrigin();
                t = numerator.dot(normal) / denom;
                return t >= 0;
            }

            return false;
        }

public:
    CollideableObject(const Eigen::Vector3f &location) :
        location(location){}

    virtual ~CollideableObject() {}

    const Eigen::Vector3f &getLocation() const {
        return location;
    }

    void setLocation(const Eigen::Vector3f &location) {
        CollideableObject::location = location;
    }

    virtual void collide(Ray &ray, Eigen::Vector3f &pointOfInterception) =0;
    virtual bool intersect(Ray &ray, Eigen::Vector3f &pointOfInterception) =0;
};

#endif // COLLIDEABLEOBJECT_H
