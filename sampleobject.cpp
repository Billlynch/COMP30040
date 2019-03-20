#include "sampleobject.h"

/*!
 * \brief SampleObject::SampleObject
 * \param location
 * \param side
 * \param normal
 * \param radius
 * \param n1
 * \param q
 * The constructor for the sample object.
 */
SampleObject::SampleObject(Eigen::Vector3d location, int side,
                           Eigen::Vector3d normal, double radius,
                           std::complex<double> n1, std::complex<double> q)
    : CollideableObject(std::move(location), side, std::move(normal)) {
  m_radius = radius;
  m_n1 = n1;
  m_q = q;
  m_i = {0, 1};
  m_my = 1.0;
  m_x = 0;
  m_n0 = {1.0, 0};
}

/*!
  *\brief SampleObject:~SampleObject()
  * The default destructor.
  */
SampleObject::~SampleObject() = default;

/*!
 * \brief SampleObject::calculatePolarisationUsingGriggsFormulae
 * \param ray
 * \param theta0
 * \param theta1
 * This generates the matrix for the Jones calculus model and updates the
 * polarisation of the Ray. The maths for this can be found in my report.
 *
 */
void SampleObject::calculatePolarisationUsingJonesCalculus(
    Ray &ray, std::complex<double> &theta0, std::complex<double> &theta1) {
  m_i = {0, 1};
  // Rpp
  m_rpp = (m_n1 * cos(theta0)) - (m_n0 * cos(theta1));
  m_rpp = m_rpp / ((m_n1 * cos(theta0)) + (m_n0 * cos(theta1)));

  std::complex<double> temp = m_i * static_cast<std::complex<double>>(2.0) *
                              m_n0 * m_n1 * cos(theta0) * sin(theta1) * m_x *
                              m_q;
  temp = temp / ((m_n1 * cos(theta0)) + (m_n1 * cos(theta1)));

  m_rpp = m_rpp - temp;

  // Rss
  m_rss = (m_n0 * cos(theta0)) - (m_n1 * cos(theta1));
  m_rss = m_rss / ((m_n0 * cos(theta0)) + (m_n1 * cos(theta1)));

  // Rsp
  m_rsp = (m_i * m_q * m_n0 * m_n1 * m_my * cos(theta0) * sin(theta1));
  m_rsp = m_rsp / ((m_n1 * cos(theta0) + m_n0 * cos(theta1)) *
                   (m_n0 * cos(theta0) + m_n1 * cos(theta1)) * cos(theta1));

  // Rps
  m_rps = -m_rsp;

  m_R << m_rpp, m_rps, m_rsp, m_rss;

  Matrix4cd newPolar = m_R * ray.getPolarisation();
  ray.setPolarisation(newPolar);
}

/*!
 * \brief SampleObject::setM_Y
 * \param m_y
 *
 * This changes the MY value for the Sample (MY is proprtional to the driven magnetic force)
 * The input is the slider value from the UI.
 */
void SampleObject::setM_Y(double h) {
    this->m_my = h;
}

/*!
 * \brief SampleObject::collide
 * \param ray
 * \param pointOfInterception
 *
 * This is called when the ray collides with the sample. We need to calculate the angle of
 * reflection and interception first, we then emit these new angles to update the 3D view.
 * Then we then update the ray's origin, direction, polarisation and running matrix product.
 * Then we emit the ray information to update the 3D view.
 */
void SampleObject::collide(Ray &ray, Eigen::Vector3d &pointOfInterception) {
  std::complex<double> theta0, theta1;
  calculateAngleOfInterception(ray, theta0);
  calculateAngleOfRefraction(theta0, theta1);
  emit newThetas(theta0, theta1);

  // calculate S
  calculatePolarisationUsingJonesCalculus(ray, theta0, theta1);

  ray.calculationMatrixMultiplication(m_R);

  ray.setDirection(ray.getDirection() -
                   2 * (ray.getDirection().dot(this->getNormal())) *
                       this->getNormal());
  ray.setOrigin(pointOfInterception +
                ray.getDirection() * 0.01f); // move it along the normal so it
                                             // won't hit the same object again
  emit outputPolarisationUpdated(ray.getPolarisation());
  emit outputDirectionUpdated(ray.getDirection());
}

/*!
 * \brief SampleObject::intersect
 * \param ray
 * \param pointOfInterception
 * \return true if a collision is detected.
 *
 * This first checks to see if the ray collides with the plane of the sample and then
 * checks to see if it within the radius of the sample.
 */
bool SampleObject::intersect(Ray &ray, Eigen::Vector3d &pointOfInterception) {
  double t;

  if (this->interceptPlane(ray, t)) {
    pointOfInterception = ray.getOrigin() + ray.getDirection() * t;
    Eigen::Vector3d v = pointOfInterception - this->getLocation();
    double d2 = v.dot(v);

    return (sqrt(d2) <= m_radius);
  }

  return false;
}

/*!
 * \brief SampleObject::getType
 * \return 3 as this denotes the type of the Sample.
 */
int SampleObject::getType() { return 3; }
