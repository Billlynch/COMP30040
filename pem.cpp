#include "pem.h"


/*!
 * \brief PEM::calculatePolarisationMatrix
 *
 * This is where the matrix for use in the Jones Calculus is generated. Based off the
 * parameters for the PEM object and it's perception of time.
 */
void PEM::calculatePolarisationMatrix() {
  std::complex<double> phi = m_phaseAmplitude * sin(m_angularFrequency * time);

  if (noiseState == 1) {
    phi *= dist(noise_gen);
  }

  std::complex<double> a = exp((m_i * phi) / 2.0);
  std::complex<double> b = exp((-m_i * phi) / 2.0);

  m_polarizationMatrix << a, 0.0, 0.0, b;
}

/*!
 * \brief PEM::PEM
 * \param location
 * \param side
 * \param normal
 * \param radius
 * \param phaseAmplitude
 * \param angularFrequency
 *
 * Constructor, initialises time at zero.
 */
PEM::PEM(Eigen::Vector3d location, int side, Eigen::Vector3d normal,
         double radius, std::complex<double> phaseAmplitude,
         std::complex<double> angularFrequency)
    : CollideableObject(std::move(location), side, std::move(normal)),
      m_phaseAmplitude(phaseAmplitude), m_angularFrequency(angularFrequency),
      m_radius(radius) {
  this->time = 0.0f;
}

/*!
 * \brief PEM::getType
 * \return 1 - which denotes a PEM.
 */
ObjectType PEM::getType() { return ObjectType::pem; }

/*!
 * \brief PEM::intersect
 * \param ray
 * \param pointOfInterception
 * \return true if an intersection is detected else false.
 *
 * We first see if this intercepts with the plane of the object with infinate width.
 * Then we see if it hit within the bounds of the radius of the PEM. This is done by
 * looking at the point of interception on the plane (where t is the distance along the
 * ray at which the interception was detected). We can use the distance from the point of
 * interception to the centre of the disc which defines the PEM and see if the square root
 * of the dot product of this value and it's self if less than that of the radius. If it is
 * then we collided with the PEM.
 */
bool PEM::intersect(Ray &ray, Eigen::Vector3d &pointOfInterception) {
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
 * \brief PEM::incrementTime
 * This is called whent the PEM timer triggers it. Increment the time value in the object.
 */
void PEM::incrementTime() { this->time++; }

/*!
 * \brief PEM::setNoiseState
 * \param state
 *
 * This enables or disables the noise for the PEM object.
 */
void PEM::setNoiseState(int state) { this->noiseState = state; }

/*!
 * \brief PEM::newNoise
 * \param d - the ditrobution
 * \param gen - the number generator
 *
 * This is where we set a new generator and distrobution for the noise calculation.
 * This is needed so that the same values which are shown on the graph are used.
 */
void PEM::newNoise(std::normal_distribution<> d, std::mt19937 gen) {
  this->dist = d;
  this->noise_gen = gen;
}

/*!
 * \brief PEM::collide
 * \param ray
 * \param pointOfInterception
 *
 * This is called if we know we have a collision. At which point we know the point of the
 * collision. We first calculate the matrix for the Jones calculus, then adjust the polarisation
 * in the ray and set the ray's origin to just past that of this object. (The ray should pass through
 * so no need to work out a new direction, just re-initialise the ray where the ray would exit the PEM)
 * we go just past the PEM to avoid double collision on exit.
 */
void PEM::collide(Ray &ray, Eigen::Vector3d &pointOfInterception) {

  calculatePolarisationMatrix();

  ray.setPolarisation(m_polarizationMatrix * ray.getPolarisation());
  ray.calculationMatrixMultiplication(m_polarizationMatrix);

  // assuming no change in direction
  ray.setOrigin(pointOfInterception +
                ray.getDirection() * 0.01f); // move it along the normal so it
                                             // won't hit the same object again

  emit outputPolarisationUpdated(ray.getPolarisation());
}

/*!
 * \brief PEM::getPolarizationMatrix
 * \return the polarision matrix at the called time for the PEM
 */
Matrix22cd PEM::getPolarizationMatrix()
{
    this->calculatePolarisationMatrix();
    return this->m_polarizationMatrix;
}
