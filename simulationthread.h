#ifndef SIMULATIONTHREAD_H
#define SIMULATIONTHREAD_H

#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <complex>
#include <vector>
#include <random>
#include <cmath>

#include <sampleobject.h>
#include <polarisingfilter.h>
#include <kerrrotationgraph.h>
#include <ray.h>
#include <pem.h>
#include <collideableobject.h>
#include <QSlider>
#include <oglwidget.h>
#include <threedimentionalvisualisation.h>


typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2>> ListMatrix4cd;


class SimulationThread : public QThread {
  Q_OBJECT
 public:
  SimulationThread(QObject* parent = nullptr);
  ~SimulationThread() override;

  void simulate(double Q_r, double Q_i, double n0_r, double n0_i, OGLWidget& representation, kerrRotationGraph& graph, ThreeDimentionalVisualisation& rep);
  void customAbort();

 protected:
  void run() override;

 public slots:
  void incrementPEMTimeProgression();
  void fireNextRay();
  void angleOfIncidenceChanged(double angle);
  void newLaserNoise(std::normal_distribution<> d, std::mt19937 gen);
  void newLaserNoiseState(int state);


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
  SampleObject* sample;
  PEM* pem;
  PolarisingFilter* polarisingFilter;

  std::vector<CollideableObject*> m_objectsInScene;

  SampleObject* setupSample(std::complex<double> n1, std::complex<double> q, OGLWidget& representation, kerrRotationGraph& graph);
  PolarisingFilter* setupPolariser(Eigen::Vector2d targetPolarisation, OGLWidget& representation);
  PEM* setupPEM(std::complex<double> amplitude, std::complex<double> phase, OGLWidget& representation);
  Matrix4cd generateInitalPolarisation();

  void trace(ListMatrix4cd& outputList, std::vector<CollideableObject*>& objectsInScene, int& rayCount);
  void castRay(Ray& ray, std::vector<CollideableObject*>& objectsInScene, int& depth);

 signals:
  void emittedNewRay(Matrix4cd polarisation);
  void emittedNewRayFromAnalyiser(Matrix4cd polarisation);

  void newPositions(Eigen::Vector3d analyiserPosition, Eigen::Vector3d rayDirection, std::vector<CollideableObject*> objectsInScene);
  void simComplete(ListMatrix4cd &polarisations);

};

#endif // SIMULATIONTHREAD_H
