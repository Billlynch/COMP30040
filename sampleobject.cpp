#include "sampleobject.h"
#include <iostream>


SampleObject::SampleObject(Eigen::Vector3d location,
                           Eigen::Vector3d normal,
                           double radius,
                           std::complex<double> n1,
                           std::complex<double> q) : CollideableObject (location)
{
    m_normal = normal;
    m_radius = radius;
    m_n1 = n1;
    m_q = q;
    m_i = {0,1};
    m_my = 1.0;
    m_x = 0;
    m_n0 = {1.0,0};
}

SampleObject::~SampleObject()
{

}

void SampleObject::calculatePolarisationUsingGriggsFormulae(Ray &ray, std::complex<double> &theta0, std::complex<double> &theta1)
{
    m_i = {0,1};
    // Rpp
    m_rpp = (m_n1 * cos(theta0)) - (m_n0 * cos(theta1));
    m_rpp = m_rpp / ((m_n1 * cos(theta0)) + (m_n0 * cos(theta1)));

    std::complex<double> temp = m_i * static_cast<std::complex<double>>(2.0) * m_n0 * m_n1 * cos(theta0) * sin(theta1) * m_x * m_q;
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

   Matrix4cd newPolar = m_R*ray.getPolarisation();
   ray.setPolarisation(newPolar);
}

void SampleObject::collide(Ray &ray, Eigen::Vector3d &pointOfInterception)
{
    std::complex<double> theta0, theta1;
    calculateAngleOfInterception(ray, theta0);
    calculateAngleOfRefraction(theta0, theta1);

    // calculate S
    calculatePolarisationUsingGriggsFormulae(ray, theta0, theta1);

    ray.calculationMatrixMultiplication(m_R);

    // assuming perfect refraction - should be theta1
    ray.setDirection(ray.getDirection() - 2 * (ray.getDirection().dot(m_normal)) * m_normal );
    ray.setOrigin(pointOfInterception + ray.getDirection()*0.01f); // move it along the normal so it won't hit the same object again
    emit outputPolarisationUpdated(ray.getPolarisation());
}

bool SampleObject::intersect(Ray &ray, Eigen::Vector3d &pointOfInterception)
{
    double t;

    if (this->interceptPlane(ray, m_normal, t))
    {
        pointOfInterception = ray.getOrigin() + ray.getDirection() * t;
        Eigen::Vector3d v = pointOfInterception - this->getLocation();
        double d2 = v.dot(v);
        if (sqrt(d2) <= m_radius)
        {
            //std::cout << "collide with sample" << std::endl;
        }
        return (sqrt(d2) <= m_radius);
    }

    return false;
}
