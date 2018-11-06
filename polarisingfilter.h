#ifndef POLARISINGFILTER_H
#define POLARISINGFILTER_H

#include "collideableobject.h"

typedef Eigen::Matrix<std::complex<double>, 2, 2> Matrix22d;

// const float degreeMulitplier = static_cast<float>(180.0 / M_PI);

class PolarisingFilter : public CollideableObject
{
    Eigen::Vector3d m_normal;
    Matrix22d m_polarizationMatrix;
    Eigen::Vector2d m_targetPolarisation;
    double m_radius;
    std::complex<double> m_n1;

public:
    PolarisingFilter(Eigen::Vector3d location,
                     Eigen::Vector3d normal,
                     double radius,
                     std::complex<double> n1,
                     Eigen::Vector2d targetPolarisation);

    ~PolarisingFilter(){}

    void collide(Ray &ray, Eigen::Vector3d &pointOfInterception);
    bool intersect(Ray &ray, Eigen::Vector3d &pointOfInterception);

protected:
    void calculatePolarisationMatrix();

    void calculateAngleOfRefraction(std::complex<double> theta0, std::complex<double> &theta1)
    {
        std::complex<double> numerator1 = m_n0 * sin(theta0);
        theta1 = asin(numerator1 / m_n1); // the angle of refraction
    }

    void calculateAngleOfInterception(Ray &ray, std::complex<double> &theta0)
    {
        m_normal.normalize();
        Eigen::Vector3d rayDirection = ray.getDirection();
        rayDirection.normalize();
        std::complex<double> numerator0 = rayDirection.dot(m_normal);
        std::complex<double> denominator0 = rayDirection.norm() * m_normal.norm();
        theta0 = acos(numerator0 / denominator0); // the angle of incidence
    }
};

#endif // POLARISINGFILTER_H