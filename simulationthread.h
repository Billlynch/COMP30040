#ifndef SIMULATIONTHREAD_H
#define SIMULATIONTHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <complex>
#include <vector>

#include <sampleobject.h>
#include <polarisingfilter.h>
#include <ray.h>
#include <pem.h>
#include <collideableobject.h>
#include <QSlider>
#include <oglwidget.h>


typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2>> ListMatrix4cd;


class SimulationThread : public QThread {
  Q_OBJECT
 public:
  SimulationThread(QObject* parent = nullptr);
  ~SimulationThread() override;

  void simulate(double Q_r, double Q_i, double n0_r, double n0_i, OGLWidget& representation);
  void customAbort();

 protected:
  void run() override;

 public slots:
  void incrementPEMTimeProgression();
  void fireNextRay();
  void angleOfIncidenceChanged(double angle);

 private:
  QMutex mutex;
  QWaitCondition condition;
  bool restart, abort;
  Eigen::Vector3d emissionPosition = Eigen::Vector3d(-0.0, -5.0, 0.0);
  Eigen::Vector3d emissionDirection;

  std::complex<double> m_q, m_n_1;
  SampleObject* sample;
  PEM* pem;
  PolarisingFilter* polarisingFilter;

  std::vector<CollideableObject*> m_objectsInScene;

  SampleObject* setupSample(std::complex<double> n1, std::complex<double> q, OGLWidget& representation);
  PolarisingFilter* setupPolariser(Eigen::Vector2d targetPolarisation, OGLWidget& representation);
  PEM* setupPEM(std::complex<double> amplitude, std::complex<double> phase, OGLWidget& representation);
  Matrix4cd generateInitalPolarisation();

  void trace(ListMatrix4cd& outputList, std::vector<CollideableObject*>& objectsInScene, int& rayCount);
  void castRay(Ray& ray, std::vector<CollideableObject*>& objectsInScene, int& depth);

 signals:
  void emittedNewRay(Matrix4cd polarisation);
  void newAnalyiserPosition(Eigen::Vector3d analyiserPosition);
  void simComplete(ListMatrix4cd polarisations);

};

#endif // SIMULATIONTHREAD_H
