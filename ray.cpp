#include "ray.h"


Ray::Ray(float amplitude, Eigen::Vector3f origin, Eigen::Vector3f direction, Matrix4cf polarisation, Eigen::Vector2cf targetPolarisation, double w):
    m_origin(origin),
    m_direction(direction),
    m_polarisation(polarisation),
    m_amplitude(amplitude),
    m_w(static_cast<float>(w)),
    m_targetPolarisation(targetPolarisation){


        Vector2cf right = Vector2cf(1.0f, 0.0f);
        m_targetPolarisation.norm();
        right.norm();
        auto temp1 = right.dot(m_targetPolarisation);
        auto temp2 = right.norm() * m_targetPolarisation.norm();
        auto a = acos(temp1 / temp2);

        float angle = static_cast<float>(a.real());

        m_calculationMatrix(0,0) = powf(cosf(angle), 2.0f);
        m_calculationMatrix(0,1) = sinf(angle) * cosf(angle);
        m_calculationMatrix(1,0) = sinf(angle) * cosf(angle);
        m_calculationMatrix(1,1) = powf(sinf(angle), 2.0f);

}

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

Matrix4cf Ray::getCalculationMatrix()
{
    return m_calculationMatrix;
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

void Ray::calculationMatrixMultiplication(Matrix4cf &x)
{
    m_calculationMatrix = m_calculationMatrix * x;
}

void Ray::flipPolarization()
{
    Ray::m_polarisation.reverse();
}
