#ifndef COLLIDEABLEOBJECT_H
#define COLLIDEABLEOBJECT_H
#include "ray.h"

#include <QObject>
#include <Dense>
#include <complex>
#include <cmath>

typedef Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2> Matrix4d;

class CollideableObject:  public QObject
{
    Q_OBJECT
    Eigen::Vector3d m_location;

protected:
    bool interceptPlane(Ray &ray, Eigen::Vector3d &normal, double &t);

public:
    double m_n0 = 1.0;

    CollideableObject(const Eigen::Vector3d &location);

    virtual ~CollideableObject();

    const Eigen::Vector3d &getLocation() const;

    void setLocation(const Eigen::Vector3d &location);

    virtual void collide(Ray &ray, Eigen::Vector3d &pointOfInterception) =0;
    virtual bool intersect(Ray &ray, Eigen::Vector3d &pointOfInterception) =0;
};

#endif // COLLIDEABLEOBJECT_H
