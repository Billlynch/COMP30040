#include "sampleobject.h"
#include <iostream>


SampleObject::SampleObject(Eigen::Vector3f location,
                           Eigen::Vector3f normal,
                           float radius,
                           std::complex<float> n1,
                           std::complex<float> q,
                           double k) : CollideableObject (location)
{
    m_normal = normal;
    m_radius = radius;
    m_n1 = n1;
    m_q = q;
    m_k = k;
    m_i = sqrt(m_i);
    m_my = 1.0f;
    m_x = 0;
}

SampleObject::~SampleObject()
{

}

void SampleObject::calculatePolarisationUsingGriggsFormulae(Ray &ray, std::complex<float> &theta0, std::complex<float> &theta1)
{
    m_i = {0,1};
    // Rpp
    m_rpp = (m_n1 * cos(theta0)) - (m_n0 * cos(theta1));
    m_rpp = m_rpp / ((m_n1 * cos(theta0)) + (m_n0 * cos(theta1)));

    std::complex<float> temp = m_i * static_cast<std::complex<float>>(2.0) * m_n0 * m_n1 * cos(theta0) * sin(theta1) * m_x * m_q;
    temp = temp / ( (m_n1 * cos(theta0)) + (m_n1 * cos(theta1)));

    m_rpp = m_rpp - temp;

    //Rss
    m_rss = (m_n0 * cos(theta0)) - (m_n1 * cos(theta1));
    m_rss = m_rss / ( ( m_n0 * cos(theta0) )  + ( m_n1 * cos(theta1) )  );

    //Rsp
    m_rsp = (m_i * m_q * m_n0 * m_n1 * m_my * cos(theta0) * sin(theta1) );
    m_rsp = m_rsp / ((m_n1 * cos(theta0) + m_n0 * cos(theta1)) * (m_n0 * cos(theta0) + m_n1 * cos(theta1)) * cos(theta1));

    //Rps
    m_rps = -m_rsp;

    m_R << m_rpp, m_rps,
         m_rsp, m_rss;

   Matrix4cf newPolar = m_R*ray.getPolarisation();
   ray.setPolarisation(newPolar);
}

void SampleObject::collide(Ray &ray, Eigen::Vector3f &pointOfInterception)
{
    std::complex<float> theta0, theta1;
    calculateAngleOfInterception(ray, theta0);
    calculateAngleOfRefraction(theta0, theta1);

    // calculate S
    calculatePolarisationUsingGriggsFormulae(ray, theta0, theta1);

    ray.calculationMatrixMultiplication(m_R);

    // assuming perfect refraction - should be theta1
    ray.setDirection(ray.getDirection() - 2 * (ray.getDirection().dot(m_normal)) * m_normal );
    ray.setOrigin(pointOfInterception + ray.getDirection()*0.01f); // move it along the normal so it won't hit the same object again
}

bool SampleObject::intersect(Ray &ray, Eigen::Vector3f &pointOfInterception)
{
    float t;

    if (this->interceptPlane(ray, m_normal, t))
    {
        pointOfInterception = ray.getOrigin() + ray.getDirection() * t;
        Eigen::Vector3f v = pointOfInterception - this->getLocation();
        float d2 = v.dot(v);
        if (sqrt(d2) <= m_radius)
        {
            std::cout << "collide with sample" << std::endl;
        }
        return (sqrt(d2) <= m_radius);
    }

    return false;
}
