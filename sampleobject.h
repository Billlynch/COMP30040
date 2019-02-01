#ifndef SAMPLEOBJECT_H
#define SAMPLEOBJECT_H

#include "collideableobject.h"
#include <map>

typedef std::pair<std::complex<double>, std::complex<double>>
    GraphItem; // p then s
typedef std::map<double, GraphItem> GraphMap;

class SampleObject : public CollideableObject {
  Q_OBJECT
private:
  double m_radius, m_my;
  std::complex<double> m_q;
  std::complex<double> m_i;
  std::complex<double> m_n0; // air
  std::complex<double> m_x;
  std::complex<double> m_n1;
  std::complex<double> m_rpp;
  std::complex<double> m_rss;
  std::complex<double> m_rsp;
  std::complex<double> m_rps;
  Matrix4d m_R;
  GraphMap m_graphMap;
  Eigen::Vector3d *m_normal_map_normal;

public:
  SampleObject(Eigen::Vector3d location, int side, Eigen::Vector3d normal,
               double radius, std::complex<double> n1, std::complex<double> q);

  ~SampleObject();

  void collide(Ray &ray, Eigen::Vector3d &pointOfInterception);
  bool intersect(Ray &ray, Eigen::Vector3d &pointOfInterception);
  int getType();
  void calculatePolarisationUsingGriggsFormulae(Ray &ray,
                                                std::complex<double> &theta0,
                                                std::complex<double> &theta1);
  void setM_Y(double m_y);
  void updatedNormalMapNormal(Eigen::Vector3d normal);

private:
  void insertIntoGraphMap(std::complex<double> angle) {
    auto originalSize = m_graphMap.size();
    GraphItem newItem = GraphItem(m_rsp / m_rpp, m_rps / m_rss);
    m_graphMap.insert(std::pair<double, GraphItem>(angle.real(), newItem));
    if (originalSize != m_graphMap.size()) {
      emit newAngleOutout(m_graphMap);
    }
  }

protected:
  void calculateAngleOfRefraction(std::complex<double> theta0,
                                  std::complex<double> &theta1) {
    std::complex<double> numerator1 = m_n0 * sin(theta0);
    theta1 = asin(numerator1 / m_n1); // the angle of refraction
  }

  void calculateAngleOfInterception(Ray &ray, std::complex<double> &theta0) {
    auto normal = this->getNormal();
    normal.normalize();
    Eigen::Vector3d faux_normal;
    if (this->m_normal_map_normal != nullptr) {
        faux_normal = normal.cross(*this->m_normal_map_normal);
    } else {
        faux_normal = normal;
    }
    Eigen::Vector3d rayDirection = ray.getDirection();
    rayDirection.normalize();
    std::complex<double> numerator0 = rayDirection.dot(faux_normal);
    std::complex<double> denominator0 = rayDirection.norm() * faux_normal.norm();
    theta0 = acos(numerator0 / denominator0); // the angle of incidence
    this->insertIntoGraphMap(theta0);
  }

signals:
  void outputPolarisationUpdated(Matrix4cd polarisation);
  void outputDirectionUpdated(Eigen::Vector3d direction);
  void newAngleOutout(GraphMap graphMap);
  void newThetas(std::complex<double> theta0, std::complex<double> theta1);

};

#endif // SAMPLEOBJECT_H
