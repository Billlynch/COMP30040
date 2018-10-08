#ifndef COLLIDEABLEOBJECT_H
#define COLLIDEABLEOBJECT_H
#include "ray.h"

#include <Core>
#include <complex>

typedef Eigen::Matrix<std::complex<float>, 2, 2, 0, 2, 2> Matrix4cf;
typedef Eigen::Matrix<std::complex<float>, 2, 1, 0, 2, 1 > Vector2cf;

class CollideableObject
{
    Eigen::Vector3f m_location;

protected:
    bool interceptPlane(Ray &ray, Eigen::Vector3f &normal, float &t);

public:
    CollideableObject(const Eigen::Vector3f &location);

    virtual ~CollideableObject();

    const Eigen::Vector3f &getLocation() const;

    void setLocation(const Eigen::Vector3f &location);

    virtual void collide(Ray &ray, Eigen::Vector3f &pointOfInterception) =0;
    virtual bool intersect(Ray &ray, Eigen::Vector3f &pointOfInterception) =0;
};

#endif // COLLIDEABLEOBJECT_H
