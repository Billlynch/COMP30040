#ifndef SAMPLE_H
#define SAMPLE_H

#include "collideableobject.h"


typedef Eigen::Matrix<std::complex<float>, 2, 2, 0, 2, 2> Matrix4cf;
typedef Eigen::Matrix<std::complex<float>, 2, 1, 0, 2, 1 > Vector2cf;


class Sample: public CollideableObject
{
    Eigen::Vector3f normal;

        std::complex<float> i = -1.0f; // gets made into square root -1 in constructor
        std::complex<float> n0 = 1.0f; // air
        std::complex<float> n1 = 0.27732f; // for gold 0.27732

        std::complex<float> rpp = 0.0f;
        std::complex<float> rss = 0.0f;
        std::complex<float> rsp = 0.0f;
        std::complex<float> rps = 0.0f;

        float q = 0.0f; // This will be varied by the handle in the ui - the measure of magnatism driven across the sample
        float my = 0.0f; // This is related to q somehow - need to work out why

    public:
        Sample(const Eigen::Vector3f location, const Eigen::Vector3f normal, std::complex<float> n1, float q) :
                CollideableObject(location) {
                    this->normal = normal;
                    this->normal.normalize();
                    i = sqrt(i);

                    this->n1 = n1;
                    this->q = q;
                }
        ~Sample() override{}

        void calculateAngleOfRefraction(std::complex<float> theta0, std::complex<float> &theta1) {
            std::complex<float> numerator1 = n0 * sin(theta0);
            theta1 = asin(numerator1 / n1); // the angle of refraction
    //        printf("The angle of refraction is: %f \n", theta1);
        }

        void calculateAngleOfInterception(Ray &ray, std::complex<float> &theta0) {
            std::complex<float> numerator0 = ray.getDirection().dot(this->normal);
            std::complex<float> denominator0 = ray.getDirection().norm() * this->normal.norm();
            theta0 = acos(numerator0 / denominator0); // the angle of incidence
    //        printf("The angle of incidence is: %f \n", theta0);
        }

        void collide(Ray &ray, Eigen::Vector3f &pointOfInterception) override {
            std::complex<float> theta0, theta1;
            calculateAngleOfInterception(ray, theta0);
            calculateAngleOfRefraction(theta0, theta1);

            // calculate rpp
            std::complex<float> numerator, denom;
            numerator = n1 * cos(theta0) - n0 * cos(theta1);
            denom = n1 * cos(theta0) + n0 * cos(theta1);
            rpp = numerator / denom;

            // calculate rss
            numerator = n0 * cos(theta0) - n1 * cos(theta1);
            denom = n0 * cos(theta0) + n1 * cos(theta1);
            rss = numerator / denom;

            // calculate rsp and rps
            denom = (n1 * cos(theta0) + n0 * cos(theta1)) * (n0 * cos(theta0) + n1 * cos(theta1)) * cos(theta1); // common denom for rsp and rps
            numerator = n0 * n1 * cos(theta0) * sin(theta1) * i * q * my;
            rsp = numerator / denom;
            numerator = n0 * n1 * cos(theta0) * sin(theta1) * -i * q * my;
            rps = numerator / denom;

            Matrix4cf R;
            R << rpp, rss,
                 rsp, rps;

    //        Vector2cf V;
    //        V << ray.getPolarisation()(0), ray.getPolarisation()(1);

            Vector2cf newPolar = R*ray.getPolarisation();
            ray.setPolarisation(newPolar);


    //        printf("rpp = %f5 + i%f5 \n", std::real(rpp), std::imag(rpp));
    //        printf("rss = %f5 + i%f5 \n", std::real(rss), std::imag(rss));
    //        printf("rsp = %f5 + i%f5 \n", std::real(rsp), std::imag(rsp));
    //        printf("rps = %f5 + i%f5 \n", std::real(rps), std::imag(rps));
        }

        // for now this object is just an infinate plane
        bool intersect(Ray &ray, Eigen::Vector3f &pointOfInterception) override {
            float t;

            // force collision for now

            if (this->interceptPlane(ray, this->normal, t))
            {
                return true;
            }

            return true;
        }
    };

#endif // SAMPLE_H
