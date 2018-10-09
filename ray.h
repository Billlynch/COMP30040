#include <Core>

#ifndef RAY_H
#define RAY_H
typedef Eigen::Matrix<std::complex<float>, 2, 1> Vector2cf;


class Ray{
    Eigen::Vector3f m_origin;
    Eigen::Vector3f m_direction;
    Vector2cf m_polarisation;
    float m_amplitude, m_w; //w is waveLength

public:
    Ray(float amplitude,
        Eigen::Vector3f origin,
        Eigen::Vector3f direction,
        Vector2cf polarisation,
        double w);

    ~Ray();


    Eigen::Vector3f getOrigin();
    Eigen::Vector3f getDirection();
    Vector2cf getPolarisation();
    float getAmplitude();
    float getWaveLength();

    void setOrigin(Eigen::Vector3f origin);
    void setDirection(Eigen::Vector3f direction);
    void setPolarisation(Vector2cf polarisation);
    void setAplitude(float amplitude);
    void setWaveLength(float w);

    void flipPolarization();
};
#endif // RAY_H
