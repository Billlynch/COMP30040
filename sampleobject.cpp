#include "sampleobject.h"

#include <iostream>



SampleObject::SampleObject(Eigen::Vector3f location,
                           Eigen::Vector3f normal,
                           float radius,
                           std::complex<float> n1,
                           float q,
                           double k) : CollideableObject (location)
{
    m_normal = normal;
    m_radius = radius;
    m_n1 = n1;
    m_q = q;
    m_k = k;
    m_i = sqrt(m_i);
}

SampleObject::~SampleObject()
{

}

void SampleObject::collide(Ray &ray, Eigen::Vector3f &pointOfInterception)
{
    std::complex<float> theta0, theta1;
    calculateAngleOfInterception(ray, theta0);
    calculateAngleOfRefraction(theta0, theta1);

    // calculate rpp
    std::complex<float> numerator, denom;
    numerator = m_n1 * cos(theta0) - m_n0 * cos(theta1);
    denom = m_n1 * cos(theta0) + m_n0 * cos(theta1);
    m_rpp = numerator / denom;

    // calculate rss
    numerator = m_n0 * cos(theta0) - m_n1 * cos(theta1);
    denom = m_n0 * cos(theta0) + m_n1 * cos(theta1);
    m_rss = numerator / denom;

    // calculate rsp and rps
    denom = (m_n1 * cos(theta0) + m_n0 * cos(theta1)) * (m_n0 * cos(theta0) + m_n1 * cos(theta1)) * cos(theta1); // common denom for rsp and rps
    numerator = m_n0 * m_n1 * cos(theta0) * sin(theta1) * m_i * m_q * m_my;
    m_rsp = numerator / denom;
    numerator = m_n0 * m_n1 * cos(theta0) * sin(theta1) * -m_i * m_q * m_my;
    m_rps = numerator / denom;

    Matrix4cf R;
    R << m_rpp, m_rss,
         m_rsp, m_rps;

    Vector2cf newPolar = R*ray.getPolarisation();
    ray.setPolarisation(newPolar);

    ray.setOrigin(pointOfInterception);
    // assuming perfect refraction
    ray.setDirection(ray.getDirection() - 2 * (ray.getDirection().dot(m_normal)) * m_normal );
}

bool SampleObject::intersect(Ray &ray, Eigen::Vector3f &pointOfInterception)
{
    float t;

    if (this->interceptPlane(ray, m_normal, t))
    {
        pointOfInterception = ray.getOrigin() + ray.getDirection() * t;
        Eigen::Vector3f v = pointOfInterception - this->getLocation();
        float d2 = v.dot(v);

        std::cout << "did it collide?: " << (sqrt(d2) <= m_radius) << std::endl;
        return (sqrt(d2) <= m_radius);
    }

    return true;
}
