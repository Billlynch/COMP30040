#include <Core>

#ifndef RAY_H
#define RAY_H
typedef Eigen::Matrix<std::complex<float>, 2, 1> Vector2cf;


class Ray{
    Eigen::Vector3f origin;
    Eigen::Vector3f direction;
    Vector2cf polarisation;
    float amplitude;

public:
    Ray(float amplitude,
        Eigen::Vector3f origin,
        Eigen::Vector3f direction,
        Vector2cf polarisation):
            amplitude(amplitude),
            origin(origin),
            direction(direction),
            polarisation(polarisation){}

    ~Ray() {}


    Eigen::Vector3f getOrigin(){
        return origin;
    }

    Eigen::Vector3f getDirection(){
        return direction;
    }

    void setOrigin(Eigen::Vector3f origin){
        this->origin = origin;
    }

    void setDirection(Eigen::Vector3f direction){
        this->direction = direction;
    }

    Vector2cf getPolarisation() {
        return this->polarisation;
    }

    void setPolarisation(Vector2cf polarisation) {
        this->polarisation = polarisation;
    }

    void flipPolarization() {
        this->polarisation.reverse();
    }

    float getAmplitude() {
        return this->amplitude;
    }

    void setAplitude(float amplitude_input) {
        this->amplitude = amplitude_input;
    }
};
#endif // RAY_H
