#ifndef SAMPLEOBJECT_H
#define SAMPLEOBJECT_H

#include "collideableobject.h"

class SampleObject : public CollideableObject {
  Q_OBJECT
 private:
  double m_radius, m_my;
  std::complex<double> m_q;
  std::complex<double> m_i;
  std::complex<double> m_n0; // air
  Eigen::Vector3d m_normal;
  std::complex<double> m_x;
  std::complex<double> m_n1;
  std::complex<double> m_rpp;
  std::complex<double> m_rss;
  std::complex<double> m_rsp;
  std::complex<double> m_rps;
  Matrix4d m_R;


 public:
  SampleObject(Eigen::Vector3d location,
               Eigen::Vector3d normal,
               double radius,
               std::complex<double> n1,
               std::complex<double> q);

  ~SampleObject();

  void collide(Ray& ray, Eigen::Vector3d& pointOfInterception);
  bool intersect(Ray& ray, Eigen::Vector3d& pointOfInterception);

  void calculatePolarisationUsingGriggsFormulae(Ray& ray, std::complex<double>& theta0, std::complex<double>& theta1);

 protected:
  void calculateAngleOfRefraction(std::complex<double> theta0, std::complex<double>& theta1) {
    std::complex<double> numerator1 = m_n0 * sin(theta0);
    theta1 = asin(numerator1 / m_n1); // the angle of refraction
  }

  void calculateAngleOfInterception(Ray& ray, std::complex<double>& theta0) {
    m_normal.normalize();
    Eigen::Vector3d rayDirection = ray.getDirection();
    rayDirection.normalize();
    std::complex<double> numerator0 = rayDirection.dot(m_normal);
    std::complex<double> denominator0 = rayDirection.norm() * m_normal.norm();
    theta0 = acos(numerator0 / denominator0); // the angle of incidence
  }

 signals:
  void outputPolarisationUpdated(Matrix4cd polarisation);
};

#endif // SAMPLEOBJECT_H
