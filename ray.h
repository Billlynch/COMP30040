#include <Core>

#ifndef RAY_H
#define RAY_H
typedef Eigen::Matrix<std::complex<float>, 2, 1> Vector2cf;
typedef std::vector<Eigen::Matrix<std::complex<float>, 2, 2, 0, 2, 2>> ListMatrix4cf;
typedef Eigen::Matrix<std::complex<float>, 2, 2, 0, 2, 2> Matrix4cf;

const float degreeMulitplier = static_cast<float>(180.0 / M_PI);


class Ray{
    Eigen::Vector3f m_origin;
    Eigen::Vector3f m_direction;
    Matrix4cf m_polarisation;
    Matrix4cf m_calculationMatrix;
    float m_amplitude, m_w; //w is waveLength
    Eigen::Vector2cf m_targetPolarisation;

public:
    Ray(float amplitude,
        Eigen::Vector3f origin,
        Eigen::Vector3f direction,
        Matrix4cf polarisation,
        Eigen::Vector2cf targetPolarisation,
        double w);

    ~Ray();


    Eigen::Vector3f getOrigin();
    Eigen::Vector3f getDirection();
    Matrix4cf getPolarisation();
    Matrix4cf getCalculationMatrix();
    float getAmplitude();
    float getWaveLength();

    void setOrigin(Eigen::Vector3f origin);
    void setDirection(Eigen::Vector3f direction);
    void setPolarisation(Matrix4cf polarisation);
    void setAplitude(float amplitude);
    void setWaveLength(float w);

    void calculationMatrixMultiplication(Matrix4cf &x);

    void flipPolarization();
};
#endif // RAY_H
