#ifndef COLLIDEABLEOBJECT_H
#define COLLIDEABLEOBJECT_H
#include "ray.h"

#include <QObject>
#include <cmath>
#include <complex>
#include <eigen3/Eigen/Dense>
#include "objecttype.h"

typedef Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2> Matrix4d; // A 2 x 2 matrix of complex numbers

/*!
 * \brief The CollideableObject class
 * This is the class which needs to be extended by any object which will be interacting with the
 * ray in the ray tracing algorithm.
 */
class CollideableObject : public QObject {
  Q_OBJECT
  Eigen::Vector3d m_location;
  Eigen::Vector3d m_normal;
  int side;
  int collisionsEnabled = 1;


protected:
  bool interceptPlane(Ray &ray, double &t);

public:
  double m_n0 = 1.0; // refractive index of air

  CollideableObject(Eigen::Vector3d location, int side, Eigen::Vector3d normal);

  virtual ~CollideableObject();

  const Eigen::Vector3d &getLocation() const;
  const Eigen::Vector3d &getNormal() const;

  void setLocation(const Eigen::Vector3d &location);

  virtual ObjectType getType() = 0;

  void newPosition(Eigen::Vector3d samplePositition, double angle,
                               Eigen::Vector3d emissionDirection);

  virtual void collide(Ray &ray, Eigen::Vector3d &pointOfInterception) = 0;
  virtual bool intersect(Ray &ray, Eigen::Vector3d &pointOfInterception) = 0;

  void setEnabled(int state);
};

#endif // COLLIDEABLEOBJECT_H
