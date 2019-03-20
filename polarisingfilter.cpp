#include <utility>

#include "polarisingfilter.h"

/*!
 * \brief PolarisingFilter::PolarisingFilter
 * \param location
 * \param side
 * \param normal
 * \param radius
 * \param n1
 * \param targetPolarisation
 *
 * Constructor for the polarisingFilter, also creates the matrix for the Jones Calculus.
 */
PolarisingFilter::PolarisingFilter(Eigen::Vector3d location, int side,
                                   Eigen::Vector3d normal, double radius,
                                   std::complex<double> n1,
                                   Eigen::Vector2d targetPolarisation)
    : CollideableObject(std::move(location), side, std::move(normal)) {
  m_radius = radius;
  m_n1 = n1;
  m_targetPolarisation = std::move(targetPolarisation);
  calculatePolarisationMatrix();
}

/*!
 * \brief PolarisingFilter::collide
 * \param ray
 * \param pointOfInterception
 *
 * This method is called on clission with a ray.
 * If the polarisation has changed we tell the hysteresis loop graph.
 * We calculate the Jones Calculus matrix again (incase noise is included), then
 * reposition the ray infront of the filter. We Trigger the 3D view's idea of the
 * rays to update based on the latest ray which has gone though the filter.
 */
void PolarisingFilter::collide(Ray &ray, Eigen::Vector3d &pointOfInterception) {
  auto tempEr = Eigen::Vector2cd(ray.getPolarisation()(0, 0),
                                 ray.getPolarisation()(1, 1));
  if (tempEr != this->Er) {
    this->Er = tempEr;
    emit outputEr(Er);
  }

  this->calculatePolarisationMatrix();
  ray.setPolarisation(m_polarizationMatrix * ray.getPolarisation());
  ray.calculationMatrixMultiplication(m_polarizationMatrix);

  // assuming no change in direction
  ray.setOrigin(pointOfInterception +
                ray.getDirection() * 0.01f); // move it along the normal so it
                                             // won't hit the same object again
  emit outputPolarisationUpdated(ray.getPolarisation());
}

/*!
 * \brief PolarisingFilter::intersect
 * \param ray
 * \param pointOfInterception
 * \return if there was a collision with the filter and this ray.
 *
 * Check if we collide with the plane of infinate width and hight at the
 * position of the filter. We then detect if the collision point was within the
 * radiius of the filter. Very similar to the implementation in the PEM.
 */
bool PolarisingFilter::intersect(Ray &ray,
                                 Eigen::Vector3d &pointOfInterception) {
  double t;

  if (this->interceptPlane(ray, t)) {
    pointOfInterception = ray.getOrigin() + ray.getDirection() * t;
    Eigen::Vector3d v = pointOfInterception - this->getLocation();
    double d2 = v.dot(v);

    return (sqrt(d2) <= m_radius);
  }

  return false;
}

int PolarisingFilter::getType() { return 2; }

void PolarisingFilter::calculatePolarisationMatrix() {
  Eigen::Vector2d down = Eigen::Vector2d(0.0, -1.0);
  m_targetPolarisation.norm();
  down.norm();
  auto temp1 = down.dot(m_targetPolarisation);
  auto temp2 = down.norm() * m_targetPolarisation.norm();
  std::complex<double> a = acos(temp1 / temp2);

  double angle = a.real();

  m_polarizationMatrix(0, 0) = pow(cos(angle), 2.0);
  m_polarizationMatrix(0, 1) = sin(angle) * cos(angle);
  m_polarizationMatrix(1, 0) = sin(angle) * cos(angle);
  m_polarizationMatrix(1, 1) = pow(sin(angle), 2.0);
}

Matrix22d PolarisingFilter::getPolarisationMatrix() {
  this->calculatePolarisationMatrix();
  return m_polarizationMatrix;
}

void PolarisingFilter::setTarget(Eigen::Vector2d target) {
  this->m_targetPolarisation = target;
}
