#include "polarisingfilter.h"
#include <iostream>


PolarisingFilter::PolarisingFilter(Eigen::Vector3f location,
                                   Eigen::Vector3f normal,
                                   float radius,
                                   std::complex<float> n1,
                                   Eigen::Vector2cf targetPolarisation) : CollideableObject (location)
{
    m_normal = normal;
    m_radius = radius;
    m_n1 = n1;
    m_targetPolarisation = targetPolarisation;
    calculatePolarisationMatrix();
}

void PolarisingFilter::collide(Ray &ray, Eigen::Vector3f &pointOfInterception)
{
    ray.setPolarisation(m_polarizationMatrix*ray.getPolarisation());
    ray.calculationMatrixMultiplication(m_polarizationMatrix);

    // assuming no change in direction
    ray.setOrigin(pointOfInterception + ray.getDirection()*0.01f); // move it along the normal so it won't hit the same object again
}

bool PolarisingFilter::intersect(Ray &ray, Eigen::Vector3f &pointOfInterception)
{
    float t;
    if (this->interceptPlane(ray, m_normal, t))
    {
        pointOfInterception = ray.getOrigin() + ray.getDirection() * t;
        Eigen::Vector3f v = pointOfInterception - this->getLocation();
        float d2 = v.dot(v);
        if (sqrt(d2) <= m_radius) {
         std::cout << "collide with polariser" << std::endl;
        }
        return (sqrt(d2) <= m_radius);
    }

    return false;
}

void PolarisingFilter::calculatePolarisationMatrix()
{
    Vector2cf right = Vector2cf(1.0f, 0.0f);
    m_targetPolarisation.norm();
    right.norm();
    auto temp1 = right.dot(m_targetPolarisation);
    auto temp2 = right.norm() * m_targetPolarisation.norm();
    auto a = acos(temp1 / temp2);

    float angle = static_cast<float>(a.real());

    m_polarizationMatrix(0,0) = powf(cosf(angle), 2.0f);
    m_polarizationMatrix(0,1) = sinf(angle) * cosf(angle);
    m_polarizationMatrix(1,0) = sinf(angle) * cosf(angle);
    m_polarizationMatrix(1,1) = powf(sinf(angle), 2.0f);
}
