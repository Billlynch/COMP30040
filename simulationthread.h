#ifndef SIMULATIONTHREAD_H
#define SIMULATIONTHREAD_H

#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QWaitCondition>
#include <cmath>
#include <complex>
#include <random>
#include <vector>

#include <QSlider>
#include <collideableobject.h>
#include <kerrrotationgraph.h>
#include <pem.h>
#include <polarisingfilter.h>
#include <ray.h>
#include <sampleobject.h>
#include <threedimentionalvisualisation.h>

typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2>>
    ListMatrix4cd;

class SimulationThread : public QThread {
  Q_OBJECT
public:
  SimulationThread(QObject *parent = nullptr);
  ~SimulationThread() override;

  void simulate(double Q_r, double Q_i, double n0_r, double n0_i,
                kerrRotationGraph &graph, ThreeDimentionalVisualisation &rep);
  void customAbort();

protected:
  void run() override;

public slots:
  void incrementPEMTimeProgression();
  void fireNextRay();
  void angleOfIncidenceChanged(double angle);
  void newLaserNoise(std::normal_distribution<> d, std::mt19937 gen);
  void newLaserNoiseState(int state);
  void newPemState(int state);
  void newPemNoise(std::normal_distribution<> d, std::mt19937 gen);
  void newPemNoiseState(int state);

private:
  int laserNoise = 0;
  QMutex mutex;
  QWaitCondition condition;
  bool restart, abort;
  Eigen::Vector3d emissionPosition = Eigen::Vector3d(0.0, -5.0, 0.0);
  Eigen::Vector3d emissionDirection = Eigen::Vector3d(1.0, 1.0, 0.0);
  std::normal_distribution<> emissionNoiseDist;
  std::mt19937 emissionNoiseGen;

  std::complex<double> m_q, m_n_1;
  SampleObject *sample = nullptr;
  PEM *pem = nullptr;
  PolarisingFilter *polarisingFilter = nullptr;

  std::vector<CollideableObject *> m_objectsInScene;

  SampleObject *setupSample(std::complex<double> n1, std::complex<double> q,
                            kerrRotationGraph &graph,
                            ThreeDimentionalVisualisation &rep);
  PolarisingFilter *setupPolariser(Eigen::Vector2d targetPolarisation,
                                   ThreeDimentionalVisualisation &rep);
  PEM *setupPEM(std::complex<double> amplitude, std::complex<double> phase,
                ThreeDimentionalVisualisation &rep);
  Matrix4cd generateInitalPolarisation();

  void trace(ListMatrix4cd &outputList,
             std::vector<CollideableObject *> &objectsInScene, int &rayCount);
  void castRay(Ray &ray, std::vector<CollideableObject *> &objectsInScene,
               int &depth);

signals:
  void emittedNewRay(Matrix4cd polarisation);
  void emittedNewRayFromAnalyiser(Matrix4cd polarisation);

  void newPositions(Eigen::Vector3d analyiserPosition,
                    Eigen::Vector3d rayDirection,
                    std::vector<CollideableObject *> objectsInScene);
  void simComplete(ListMatrix4cd &polarisations);
};

#endif // SIMULATIONTHREAD_H
