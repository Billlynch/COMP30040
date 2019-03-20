#include "ray.h"

/*!
 * \brief Ray::Ray
 * \param origin
 * \param direction
 * \param polarisation
 * \param targetPolarisation
 * The constructor for the ray, we need a direction, origin and polarisation. Because
 * the concept of the laser does not exist in this sim, the constructor of the ray is the
 * laser, for that reason we need to have a target polarisation for it too. The is generates the
 * martix for use in the Jones Calculus formula.
 */
Ray::Ray(Eigen::Vector3d origin, Eigen::Vector3d direction,
         Matrix4cd polarisation, Eigen::Vector2d targetPolarisation)
    : m_origin(std::move(origin)), m_direction(std::move(direction)),
      m_polarisation(std::move(polarisation)),
      m_targetPolarisation(std::move(targetPolarisation)) {

  Eigen::Vector2d right = Eigen::Vector2d(1.0, 0.0);
  m_targetPolarisation.norm();
  right.norm();
  auto temp1 = right.dot(m_targetPolarisation);
  auto temp2 = right.norm() * m_targetPolarisation.norm();
  std::complex<double> a = acos(temp1 / temp2);

  double angle = a.real();

  m_calculationMatrix(0, 0) = pow(cos(angle), 2.0);
  m_calculationMatrix(0, 1) = sin(angle) * cos(angle);
  m_calculationMatrix(1, 0) = sin(angle) * cos(angle);
  m_calculationMatrix(1, 1) = pow(sin(angle), 2.0);
}

/*!
  *\brief Ray:~Ray()
  * The default destructor, no need to
  */
Ray::~Ray() = default;

/*!
 * \brief Ray::getOrigin
 * \return the origin of the ray
 */
Eigen::Vector3d Ray::getOrigin() { return Ray::m_origin; }

/*!
 * \brief Ray::getDirection
 * \return The direction of the ray
 */
Eigen::Vector3d Ray::getDirection() { return Ray::m_direction; }

/*!
 * \brief Ray::getPolarisation
 * \return The polarisation of the ray
 */
Matrix4cd Ray::getPolarisation() { return Ray::m_polarisation; }

/*!
 * \brief Ray::getCalculationMatrix
 * \return The current product of all the Jones Calculus applied to the ray.
 */
Matrix4cd Ray::getCalculationMatrix() { return m_calculationMatrix; }

/*!
 * \brief Ray::setOrigin
 * \param origin
 */
void Ray::setOrigin(Eigen::Vector3d origin) {
  Ray::m_origin = std::move(origin);
}

/*!
 * \brief Ray::setDirection
 * \param direction
 */
void Ray::setDirection(Eigen::Vector3d direction) {
  Ray::m_direction = std::move(direction);
}

/*!
 * \brief Ray::setPolarisation
 * \param polarisation
 */
void Ray::setPolarisation(Matrix4cd polarisation) {
  Ray::m_polarisation = std::move(polarisation);
}

/*!
 * \brief Ray::calculationMatrixMultiplication
 * \param x - the next matrix to be applied to the ray
 * This performs the calculation of the running product for
 * Jones Calculus
 */
void Ray::calculationMatrixMultiplication(Matrix4cd &x) {
  m_calculationMatrix = m_calculationMatrix * x;
}
