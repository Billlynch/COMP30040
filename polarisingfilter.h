#ifndef POLARISINGFILTER_H
#define POLARISINGFILTER_H

#include "collideableobject.h"

typedef Eigen::Matrix<std::complex<float>, 2, 2> Matrix22cf;

// const float degreeMulitplier = static_cast<float>(180.0 / M_PI);

class PolarisingFilter : public CollideableObject
{
    Eigen::Vector3f m_normal;
    Matrix22cf m_polarizationMatrix;
    Eigen::Vector2cf m_targetPolarisation;
    float m_radius;
    std::complex<float> m_n1;

public:
    PolarisingFilter(Eigen::Vector3f location,
                              Eigen::Vector3f normal,
                              float radius,
                              std::complex<float> n1,
                               Eigen::Vector2cf targetPolarisation);

    ~PolarisingFilter(){}

    void collide(Ray &ray, Eigen::Vector3f &pointOfInterception);
    bool intersect(Ray &ray, Eigen::Vector3f &pointOfInterception);

protected:
    void calculatePolarisationMatrix();

    void calculateAngleOfRefraction(std::complex<float> theta0, std::complex<float> &theta1)
    {
        std::complex<float> numerator1 = m_n0 * sin(theta0);
        theta1 = asin(numerator1 / m_n1); // the angle of refraction
    }

    void calculateAngleOfInterception(Ray &ray, std::complex<float> &theta0)
    {
        m_normal.normalize();
        Eigen::Vector3f rayDirection = ray.getDirection();
        rayDirection.normalize();
        std::complex<float> numerator0 = rayDirection.dot(m_normal);
        std::complex<float> denominator0 = rayDirection.norm() * m_normal.norm();
        theta0 = acos(numerator0 / denominator0); // the angle of incidence
    }
};

#endif // POLARISINGFILTER_H
