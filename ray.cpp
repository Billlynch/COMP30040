#include "ray.h"


Ray::Ray(float amplitude, Eigen::Vector3f origin, Eigen::Vector3f direction, Vector2cf polarisation):
    m_origin(origin),
    m_direction(direction),
    m_polarisation(polarisation),
    m_amplitude(amplitude){}

Ray::~Ray()
{

}

Eigen::Vector3f Ray::getOrigin()
{
    return Ray::m_origin;
}

Eigen::Vector3f Ray::getDirection()
{
    return Ray::m_direction;
}

Vector2cf Ray::getPolarisation()
{
    return Ray::m_polarisation;
}

float Ray::getAmplitude()
{
    return Ray::m_amplitude;
}

void Ray::setOrigin(Eigen::Vector3f origin)
{
    Ray::m_origin = origin;
}

void Ray::setDirection(Eigen::Vector3f direction)
{
    Ray::m_direction = direction;
}

void Ray::setPolarisation(Vector2cf polarisation)
{
    Ray::m_polarisation = polarisation;
}

void Ray::setAplitude(float amplitude)
{
    Ray::m_amplitude = amplitude;
}

void Ray::flipPolarization()
{
    Ray::m_polarisation.reverse();
}
