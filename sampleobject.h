#ifndef SAMPLEOBJECT_H
#define SAMPLEOBJECT_H

#include "collideableobject.h"
#include <map>
#include <utility>

typedef std::pair<std::complex<double>, std::complex<double>>
    GraphItem; // p then s
typedef std::map<double, GraphItem> GraphMap;

/*!
 * \brief The SampleObject class
 * This defines all the methods the sample object will implement, it also has some
 * implementations for things that will not change.
 */
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

public:
  SampleObject(Eigen::Vector3d location, int side, Eigen::Vector3d normal,
               double radius, std::complex<double> n1, std::complex<double> q);

  ~SampleObject();

  void collide(Ray &ray, Eigen::Vector3d &pointOfInterception);
  bool intersect(Ray &ray, Eigen::Vector3d &pointOfInterception);
  ObjectType getType();
  void calculatePolarisationUsingJonesCalculus(Ray &ray,
                                                std::complex<double> &theta0,
                                                std::complex<double> &theta1);
  void setM_Y(double m_y);

private:
  /*!
   * \brief insertIntoGraphMap
   * \param angle
   * This buts the angles and the current values from the Jones Matrix into
   * a map ready to be shown in the hysteresis loop graph.
   */
  void insertIntoGraphMap(std::complex<double> angle) {
    auto originalSize = m_graphMap.size();
    GraphItem newItem = GraphItem(m_rsp / m_rpp, m_rps / m_rss);
    std::pair<GraphMap::iterator, bool > result;

    result = m_graphMap.insert(std::pair<double, GraphItem>(angle.real(), newItem));

    if (result.second == false) { // the angle has already been done and graphed
        if  (result.first->second != newItem) { // are the s and p values the different?
            result.first->second = newItem; // replace the values
            emit newAngleOutout(m_graphMap); // update the graph
        }
    } else { // insertion was successful so update the graph
        emit newAngleOutout(m_graphMap);
    }
  }

protected:
  /*!
   * \brief calculateAngleOfRefraction
   * \param theta0
   * \param theta1
   * This calculates the angle od refraction, given the angle of incidence, useing Snell's Law
   */
  void calculateAngleOfRefraction(std::complex<double> theta0,
                                  std::complex<double> &theta1) {
    std::complex<double> numerator1 = m_n0 * sin(theta0);
    theta1 = asin(numerator1 / m_n1); // the angle of refraction
  }

  /*!
   * \brief calculateAngleOfInterception
   * \param ray
   * \param theta0
   *
   * This calculates the angle of incidence by using trig on the normal vector of the sample
   * and the direction of the ray.
   */
  void calculateAngleOfInterception(Ray &ray, std::complex<double> &theta0) {
    auto normal = this->getNormal();
    normal.normalize();
    Eigen::Vector3d rayDirection = ray.getDirection();
    rayDirection.normalize();
    std::complex<double> numerator0 = rayDirection.dot(normal);
    std::complex<double> denominator0 = rayDirection.norm() * normal.norm();
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
