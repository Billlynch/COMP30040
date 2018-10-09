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
    m_my = 1.0f;
}

SampleObject::~SampleObject()
{

}

void SampleObject::calculatePolarisationUsingGriggsFormulae(Ray &ray, std::complex<float> &theta0, std::complex<float> &theta1)
{
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
    R << m_rpp, m_rps,
         m_rsp, m_rss;

   Matrix4cf newPolar = R*ray.getPolarisation();
   ray.setPolarisation(newPolar);
}

void SampleObject::collide(Ray &ray, Eigen::Vector3f &pointOfInterception)
{
    std::complex<float> theta0, theta1;
    calculateAngleOfInterception(ray, theta0);
    calculateAngleOfRefraction(theta0, theta1);

    // calculate rpp
    calculatePolarisationUsingGriggsFormulae(ray, theta0, theta1);

    // assuming perfect refraction - should be theta1
    ray.setDirection(ray.getDirection() - 2 * (ray.getDirection().dot(m_normal)) * m_normal );
    ray.setOrigin(pointOfInterception + ray.getDirection()*0.01f); // move it along the normal so it won't hit the same object again
}

bool SampleObject::intersect(Ray &ray, Eigen::Vector3f &pointOfInterception)
{
    float t;

    if (this->interceptPlane(ray, m_normal, t))
    {
        std::cout << "It collided with the plane" << std::endl;

        pointOfInterception = ray.getOrigin() + ray.getDirection() * t;
        Eigen::Vector3f v = pointOfInterception - this->getLocation();
        float d2 = v.dot(v);

        std::cout << "did it collide in the radius?: " << (sqrt(d2) <= m_radius) << std::endl;
        return (sqrt(d2) <= m_radius);
    }

    return false;
}
