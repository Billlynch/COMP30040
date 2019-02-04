#include <utility>

#include "simulationthread.h"
#include <cmath>
#include <iostream>

SimulationThread::SimulationThread(QObject *parent) : QThread(parent) {
  restart = false;
  abort = false;
}

SimulationThread::~SimulationThread() {
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

void SimulationThread::customAbort() {
  this->abort = true;
  condition.wakeOne();
  wait();
  this->abort = false;
}

void SimulationThread::simulate(double Q_r, double Q_i, double n0_r,
                                double n0_i, kerrRotationGraph &graph,
                                ThreeDimentionalVisualisation &rep) {
  QMutexLocker locker(&mutex);

  connect(this, &SimulationThread::newPositions, &rep,
          &ThreeDimentionalVisualisation::newPositions);

  m_q = {Q_r, Q_i};
  m_n_1 = {n0_r, n0_i};

  // setup the sample
  sample = setupSample(m_n_1, m_q, graph, rep);
  // setup the Polariser
  polarisingFilter = setupPolariser(Eigen::Vector2d(1.0, 1.0), rep);
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

PolarisingFilter *
SimulationThread::setupPolariser(Eigen::Vector2d targetPolarisation,
                                 ThreeDimentionalVisualisation &rep) {
  PolarisingFilter *tempPolarisingFilter =
      new PolarisingFilter(Eigen::Vector3d(1.0, 1.0, 0.0),
                           1, // side
                           Eigen::Vector3d(0.0, 1.0, 0.0), 1.0,
                           1.0, // no refractive index for now
                           std::move(targetPolarisation));

  rep.polariserObject = tempPolarisingFilter;

  return tempPolarisingFilter;
}

PEM *SimulationThread::setupPEM(std::complex<double> amplitude,
                                std::complex<double> phase,
                                ThreeDimentionalVisualisation &rep) {
  PEM *tempPEM = new PEM(Eigen::Vector3d(-1.0, 1.0, 0.0),
                         -1, // side
                         Eigen::Vector3d(0.0, 1.0, 0.0), 5.0, phase, amplitude);

  rep.pemObject = tempPEM;

  return tempPEM;
}

Matrix4cd SimulationThread::generateInitalPolarisation() {
  std::cout << this->emissionNoiseDist(this->emissionNoiseGen) << std::endl;

  std::complex<double> Epp =
      1.0 * (this->laserNoise ? this->emissionNoiseDist(this->emissionNoiseGen)
                              : 1.0);
  std::complex<double> Esp = 0.0;
  std::complex<double> Eps = 0.0;
  std::complex<double> Ess =
      1.0 * (this->laserNoise ? this->emissionNoiseDist(this->emissionNoiseGen)
                              : 1.0);
  Matrix4cd polar;
  polar << Epp, Eps, Esp, Ess;

  return polar;
}

void SimulationThread::castRay(Ray &ray,
                               std::vector<CollideableObject *> &objectsInScene,
                               int &depth) {
  if (depth < 5) {
    depth++;
    Eigen::Vector3d pointOfInterception = Eigen::Vector3d(0.0, 0.0, 0.0);

    for (unsigned int j = 0; j < objectsInScene.size(); j++) {
      if (objectsInScene[j]->intersect(ray, pointOfInterception)) {
        objectsInScene[j]->collide(ray, pointOfInterception);
        if (this->newNormalMapVector == 1 && objectsInScene[j]->getType() == 3) // if need to update and just hit the sample
        {
            this->calculateNewPositions(ray);
            this->newNormalMapVector = 0;
        }
        castRay(ray, objectsInScene, depth);
      }
    }
  }
}

void SimulationThread::calculateNewPositions(Ray ray)
{
    auto dir = ray.getDirection().normalized();
    this->pem->setLocation(this->sample->getLocation() - (dir * 10));

    // notifity the visualisation
    emit newPositions(this->emissionPosition, this->emissionDirection,
                      this->m_objectsInScene);
}

void SimulationThread::incrementPEMTimeProgression() {
  mutex.lock();
  this->pem->incrementTime();
  mutex.unlock();
}

void SimulationThread::fireNextRay() {
  mutex.lock();

  ListMatrix4cd outputFromTrace;
  Matrix4d n;
  n << 1.0, 0.0, 0.0, 1.0;
  outputFromTrace = {n};
  Matrix4cd polar = generateInitalPolarisation();
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
  mutex.unlock();
}

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

void SimulationThread::newLaserNoise(std::normal_distribution<> d,
                                     std::mt19937 gen) {
  this->emissionNoiseDist = d;
  this->emissionNoiseGen = gen;
}

void SimulationThread::newLaserNoiseState(int state) {
  this->laserNoise = state;
}

void SimulationThread::newPemState(int state) {
  if (this->pem != nullptr) {
    this->pem->setEnabled(state);
  }
}

void SimulationThread::newPolariserState(int state) {
  if (this->polarisingFilter != nullptr) {
    this->polarisingFilter->setEnabled(state);
  }
}

void SimulationThread::newPemNoise(std::normal_distribution<> d,
                                   std::mt19937 gen) {
  if (this->pem != nullptr) {
    this->pem->newNoise(d, gen);
  }
}

void SimulationThread::newPemNoiseState(int state) {
  if (this->pem != nullptr) {
    this->pem->setNoiseState(state);
  }
}

void SimulationThread::newPolarNoise(std::normal_distribution<> d,
                                     std::mt19937 gen) {
  if (this->polarisingFilter != nullptr) {
    this->polarisingFilter->newNoise(d, gen);
  }
}

void SimulationThread::newPolarNoiseState(int state) {
  if (this->polarisingFilter != nullptr) {

    this->polarisingFilter->setNoiseState(state);
  }
}

void SimulationThread::newMyValue(double my)
{
    if (this->sample != nullptr) {
        this->sample->setM_Y(my);
    }
}

void SimulationThread::newNormalFromNormalMap(Eigen::Vector3d normal)
{
    this->sample->updatedNormalMapNormal(std::move(normal));
    this->newNormalMapVector = 1;
}

void SimulationThread::run() {
  forever {
    if (abort) {
      return;
    }
  }
}
