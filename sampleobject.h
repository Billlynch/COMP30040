#ifndef SAMPLEOBJECT_H
#define SAMPLEOBJECT_H

#include "collideableobject.h"

class SampleObject : public CollideableObject
{
private:
    float m_radius, m_q, m_my;
    double m_k; // k = extinction coefficient
    std::complex<float> m_i = -1.0f;
    const std::complex<float> m_n0 = 1.0f; // air
    Eigen::Vector3f m_normal;
    std::complex<float> m_n1;
    std::complex<float> m_rpp;
    std::complex<float> m_rss;
    std::complex<float> m_rsp;
    std::complex<float> m_rps;


public:
    SampleObject(Eigen::Vector3f location,
                 Eigen::Vector3f normal,
                 float radius,
                 std::complex<float> n1,
                 float q,
                 double k);

    ~SampleObject();

    void collide(Ray &ray, Eigen::Vector3f &pointOfInterception);
    bool intersect(Ray &ray, Eigen::Vector3f &pointOfInterception);

    void calculatePolarisationUsingGriggsFormulae(Ray &ray, std::complex<float> &theta0, std::complex<float> &theta1);

protected:
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

#endif // SAMPLEOBJECT_H
