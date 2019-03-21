#include "simulationthread.h"

/*!
 * \brief SimulationThread::SimulationThread
 * \param parent
 * The constructor, sets the abort state and restart state to false.
 */
SimulationThread::SimulationThread(QObject *parent) : QThread(parent) {
  restart = false;
  abort = false;
}

/*!
 * \brief SimulationThread::~SimulationThread
 * This clears the memory of the simulation thread. We need a mutex lock so we don't call this
 * and lose some in progress computations.
 */
SimulationThread::~SimulationThread() {
  // This allows the last ray to run through to the end by stopping any new ray being emitted.
  mutex.lock();
  abort = true;
  condition.wakeOne();
  mutex.unlock();
  wait();

  if (sample != nullptr) {
    delete sample;
    delete pem;
    delete polarisingFilter;
  }
}

/*!
 * \brief SimulationThread::safeAbort
 * This safely stops the simulation and allows the last pass of the ray tracing algorithm to complete.
 */
void SimulationThread::safeAbort() {
  this->abort = true;
  condition.wakeOne();
  wait();
  this->abort = false;
}

/*!
 * \brief SimulationThread::simulate
 * \param Q_r
 * \param Q_i
 * \param n0_r
 * \param n0_i
 * \param graph
 * \param rep
 * \param loop_graph
 *
 * This is the method called to start the simulation. We need to pass in the
 * UI elements that the simulation will update so that we have a reference to make the connection.
 * In here we take the params from the UI and setup the elements for the ray tacing algorithm.
 * We then set the thread going.
 */
void SimulationThread::simulate(double Q_r, double Q_i, double n0_r,
                                double n0_i, kerrRotationGraph &graph,
                                ThreeDimentionalVisualisation &rep,
                                Loop_graph &loop_graph) {
  QMutexLocker locker(&mutex);

  connect(this, &SimulationThread::newPositions, &rep,
          &ThreeDimentionalVisualisation::newPositions);

  m_q = {Q_r, Q_i};
  m_n_1 = {n0_r, n0_i};

  // setup the sample
  sample = setupSample(m_n_1, m_q, graph, rep);
  // setup the Polariser
  polarisingFilter = setupPolariser(Eigen::Vector2d(1.0, 1.0), rep, loop_graph);
  // setup the PEM
  pem = setupPEM(50200, 2.405, rep);

  m_objectsInScene = {pem, sample, polarisingFilter};

  if (!isRunning()) {
    start(LowPriority);
  } else {
    restart = true;
    condition.wakeOne();
  }
}

/*!
 * \brief SimulationThread::setupSample
 * \param n1
 * \param q
 * \param graph
 * \param rep
 * \return The Sample object
 *
 * Here we use the params given to build the sample object, we use this area to
 * setup the connections with the 3D view and the kerr rotation graph too.
 */
SampleObject *
SimulationThread::setupSample(std::complex<double> n1, std::complex<double> q,
                              kerrRotationGraph &graph,
                              ThreeDimentionalVisualisation &rep) {
  SampleObject *tempSample =
      new SampleObject(Eigen::Vector3d(0.0, 10.0, 0.0), // location
                       0,                               // side
                       Eigen::Vector3d(0.0, 1.0, 0.0),  // normal
                       10.0,                            // radius
                       n1,                              // refractive index
                       q);                              // Q value

  connect(tempSample, &SampleObject::newAngleOutout, &graph,
          &kerrRotationGraph::updateSeries);

  rep.sampleObject = tempSample;

  connect(tempSample, &SampleObject::newThetas, &rep,
          &ThreeDimentionalVisualisation::newThetas);

  return tempSample;
}

/*!
 * \brief SimulationThread::setupPolariser
 * \param targetPolarisation
 * \param rep
 * \param loop_graph
 * \return The polarising filter built
 *
 *This also sets up the connection between this and the hysteresis loop graph
 */
PolarisingFilter *
SimulationThread::setupPolariser(Eigen::Vector2d targetPolarisation,
                                 ThreeDimentionalVisualisation &rep, Loop_graph &hysteresis_graph) {
  PolarisingFilter *tempPolarisingFilter =
      new PolarisingFilter(Eigen::Vector3d(1.0, 1.0, 0.0),
                           1, // side
                           Eigen::Vector3d(0.0, 1.0, 0.0), 1.0,
                           1.0, // no refractive index for now
                           std::move(targetPolarisation));

  rep.polariserObject = tempPolarisingFilter;

  connect(tempPolarisingFilter, &PolarisingFilter::outputEr, &hysteresis_graph,
          &Loop_graph::updateSeries);

  return tempPolarisingFilter;
}
/*!
 * \brief SimulationThread::setupPEM
 * \param amplitude
 * \param phase
 * \param rep
 * \return The built PEM.
 */
PEM *SimulationThread::setupPEM(std::complex<double> amplitude,
                                std::complex<double> phase,
                                ThreeDimentionalVisualisation &rep) {
  PEM *tempPEM = new PEM(Eigen::Vector3d(-1.0, 1.0, 0.0),
                         -1, // side
                         Eigen::Vector3d(0.0, 1.0, 0.0), 5.0, phase, amplitude);

  rep.pemObject = tempPEM;

  return tempPEM;
}

/*!
 * \brief SimulationThread::generateInitalPolarisation
 * \return init polarisation matrix
 *
 * This sets the init polarisation from the 'laser' to be 45 degrees, i.e. the vector (1,1)
 * we need this to be in matrix form with s and p being the top left and bottom right respectivly.
 * This is where any noise is applied.
 */
Matrix4cd SimulationThread::generateInitalPolarisation() {
  std::complex<double> Epp =
      1.0 * (this->laserNoise ? this->emissionNoiseDist(this->emissionNoiseGen)
                              : 1.0); // if noise then noise value, else 1
  std::complex<double> Esp = 0.0;
  std::complex<double> Eps = 0.0;
  std::complex<double> Ess =
      1.0 * (this->laserNoise ? this->emissionNoiseDist(this->emissionNoiseGen)
                              : 1.0); // if nouse then noise value, else 1
  Matrix4cd polar;
  polar << Epp, Eps, Esp, Ess;

  return polar;
}

/*!
 * \brief SimulationThread::castRay
 * \param ray
 * \param objectsInScene
 * \param depth
 *
 * This is the recursive function that actually does the ray tracing algorithm.
 * We have a fixed depth of 5, which means we stop evaluating a ray after a maximum of 5 collisions.
 * because we are starting from 0. If we are in the depth allowed we reset the point of interception
 * and look at all objects in the scene to see if we have an intersection with any, if we do have one
 * then we apply the collision to the ray and call the same function again with the updated ray and depth.
 * This has no concept of shadowing so the order in which items are added to the objectsInScene list matters.
 */
void SimulationThread::castRay(Ray &ray,
                               std::vector<CollideableObject *> &objectsInScene,
                               int &depth) {
  if (depth < 5) {
    depth++;
    Eigen::Vector3d pointOfInterception = Eigen::Vector3d(0.0, 0.0, 0.0);

    for (unsigned int j = 0; j < objectsInScene.size(); j++) {
      if (objectsInScene[j]->intersect(ray, pointOfInterception)) {
        objectsInScene[j]->collide(ray, pointOfInterception);
        castRay(ray, objectsInScene, depth);
      }
    }
  }
}

/*!
 * \brief SimulationThread::incrementPEMTimeProgression
 * Here we pause the simulation and update the time in the PEM.
 */
void SimulationThread::incrementPEMTimeProgression() {
  mutex.lock();
  this->pem->incrementTime();
  mutex.unlock();
}

/*!
 * \brief SimulationThread::fireNextRay
 * This is called when the event loop timer triggers it. This locks the current
 * value for the time of the PEM and the positions of the objects while the ray is
 * being processed. We first create out ray, then tell the listening 3D view about it.
 * We then begin the recursive call to the castRay method. Once the ray has been computed
 * we push the informaition to the 3D view and the digrams / charts which are listening.
 * We then unpause the rest of the thread so updates to the angle and time etc can be
 * processed.
 */
void SimulationThread::fireNextRay() {
  mutex.lock();

  ListMatrix4cd outputFromTrace;
  Matrix4d n;
  n << 1.0, 0.0, 0.0, 1.0;
  outputFromTrace = {n};
  Matrix4cd polar = generateInitalPolarisation();
  outputFromTrace.push_back(polar);
  int depth = 0;

  Ray *ray = new Ray(this->emissionPosition, this->emissionDirection, polar,
                     Eigen::Vector2d(1.0, 1.0));
  emit emittedNewRay(ray->getPolarisation());
  emit emittedNewRayFromLaser(*ray);
  emit emittedNewRayFromAnalyiser(ray->getPolarisation());
  castRay(*ray, m_objectsInScene, depth);
  ray->setPolarisation(ray->getCalculationMatrix() * polar);
  outputFromTrace.push_back(ray->getPolarisation());
  delete ray;

  emit simComplete(outputFromTrace);
  outputFromTrace.clear();
  mutex.unlock();
}

/*!
 * \brief SimulationThread::angleOfIncidenceChanged
 * \param angle
 *
 * This waits for the thread to be in a state where it can deal with the change of
 * the angle of incidence, (I.E. no ray is being processed) We then use trig to calculate
 * the direction that would be needed to make a collision angle of the given angle. We
 * set the init ray direction to that and also using trig workout the new positions for the
 * other objects in the scene so that they will be collided with. We then tell the 3D view
 * about this so that the visualisation is updated to reflect this.
 */
void SimulationThread::angleOfIncidenceChanged(double angle) {
  mutex.lock();

  // analysiser
  Eigen::Vector3d positionOfSample = Eigen::Vector3d(0.0, 10.0, 0.0);
  double adjLength = positionOfSample(1) - this->emissionPosition(1);
  double oppLength = std::tan(angle * (M_PI / 180.0)) * adjLength;
  this->emissionPosition(0) = -oppLength;

  // set the new angle direction for the ray
  this->emissionDirection =
      this->sample->getLocation() - this->emissionPosition;
  // other objects
  foreach (CollideableObject *obj, this->m_objectsInScene) {
    obj->newPosition(positionOfSample, angle, this->emissionDirection);
  }

  // notifity the visualisation
  emit newPositions(this->emissionPosition, this->emissionDirection,
                    this->m_objectsInScene);

  mutex.unlock();
}

/*!
 * \brief SimulationThread::newLaserNoise
 * \param d
 * \param gen
 * This sets the noise for the laser generation.
 */
void SimulationThread::newLaserNoise(std::normal_distribution<> d,
                                     std::mt19937 gen) {
  this->emissionNoiseDist = d;
  this->emissionNoiseGen = gen;
}

/*!
 * \brief SimulationThread::newLaserNoiseState
 * \param state
 * Apply the state of the laser noise
 */
void SimulationThread::newLaserNoiseState(int state) {
  this->laserNoise = state;
}

/*!
 * \brief SimulationThread::newPemState
 * \param state
 * Set the instance to enabled or disabled for the PEM
 */
void SimulationThread::newPemState(int state) {
  if (this->pem != nullptr) {
    this->pem->setEnabled(state);
  }
}

/*!
 * \brief SimulationThread::newPolariserState
 * \param state
 * Set the instance to enabled or disabled for the polariser
 */
void SimulationThread::newPolariserState(int state) {
  if (this->polarisingFilter != nullptr) {
    this->polarisingFilter->setEnabled(state);
  }
}

/*!
 * \brief SimulationThread::newPemNoise
 * \param d
 * \param gen
 * Set the instances noise generator and distrobution for the PEM
 */
void SimulationThread::newPemNoise(std::normal_distribution<> d,
                                   std::mt19937 gen) {
  if (this->pem != nullptr) {
    this->pem->newNoise(d, gen);
  }
}

/*!
 * \brief SimulationThread::newPemNoiseState
 * \param state
 * Set the instances noise state for the PEM
 */
void SimulationThread::newPemNoiseState(int state) {
  if (this->pem != nullptr) {
    this->pem->setNoiseState(state);
  }
}

/*!
 * \brief SimulationThread::newHValue
 * \param my
 *
 * Called when teh H value is changed - MY is proportional to H so we
 * just put it into the sample instance.
 */
void SimulationThread::newHValue(double my)
{
    if (this->sample != nullptr) {
        this->sample->setM_Y(my);
    }
}

/*!
 * \brief SimulationThread::newPolarisationTarget
 * \param target
 * Sets the target angle for the polariser instance
 */
void SimulationThread::newPolarisationTarget(Eigen::Vector2d target)
{
    if (this->polarisingFilter != nullptr) {
        this->polarisingFilter->setTarget(target);
    }
}

/*!
 * \brief SimulationThread::run
 * This is just an overrided method to quit the thread if the abort flag is set
 */
void SimulationThread::run() {
  forever {
    if (abort) {
      return;
    }
  }
}
