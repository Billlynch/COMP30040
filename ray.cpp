#include "ray.h"


Ray::Ray(float amplitude, Eigen::Vector3f origin, Eigen::Vector3f direction, Matrix4cf polarisation, double w):
    m_origin(origin),
    m_direction(direction),
    m_polarisation(polarisation),
    m_amplitude(amplitude),
    m_w(static_cast<float>(w)){}

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

Matrix4cf Ray::getPolarisation()
{
    return Ray::m_polarisation;
}

float Ray::getAmplitude()
{
    return Ray::m_amplitude;
}

float Ray::getWaveLength()
{
    return Ray::m_w;
}

void Ray::setOrigin(Eigen::Vector3f origin)
{
    Ray::m_origin = origin;
}

void Ray::setDirection(Eigen::Vector3f direction)
{
    Ray::m_direction = direction;
}

void Ray::setPolarisation(Matrix4cf polarisation)
{
    Ray::m_polarisation = polarisation;
}

void Ray::setAplitude(float amplitude)
{
    Ray::m_amplitude = amplitude;
}

void Ray::setWaveLength(float w)
{
    Ray::m_w = w;
}

void Ray::flipPolarization()
{
    Ray::m_polarisation.reverse();
}
