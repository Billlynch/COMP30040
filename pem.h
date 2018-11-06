#ifndef PEM_H
#define PEM_H

#include "collideableobject.h"


typedef Eigen::Matrix<std::complex<double>, 2, 2> Matrix22cd;

class PEM :  public CollideableObject
{
private:
      Matrix22cd m_polarizationMatrix;
      std::complex<double> m_phaseAmplitude;
      std::complex<double> m_angularFrequency;
      Eigen::Vector3d m_normal;
      double time, m_radius;
      std::complex<double> m_i = {0,1};

      void calculatePolarisationMatrix();

public:
      PEM(Eigen::Vector3d location,
          Eigen::Vector3d normal,
          double radius,
          std::complex<double> phaseAmplitude,
          std::complex<double> angularFrequency);

      ~PEM(){}

      void collide(Ray &ray, Eigen::Vector3d &pointOfInterception);
      bool intersect(Ray &ray, Eigen::Vector3d &pointOfInterception);
};

#endif // PEM_H
