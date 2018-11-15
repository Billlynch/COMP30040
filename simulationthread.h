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
#include <oglwidget.h>


typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2>> ListMatrix4cd;


class SimulationThread : public QThread
{
    Q_OBJECT
public:
    SimulationThread(QObject *parent = nullptr);
    ~SimulationThread() override;

    void simulate(double Q_r, double Q_i, double n0_r, double n0_i, int rayCount, OGLWidget &representation);
    void customAbort();

protected:
    void run() override;

signals:
    void simComplete(ListMatrix4cd polarisations);

public slots:
    void incrementPEMTimeProgression();
    void fireNextRay();
    void receiveUpdatedPolarisationFromPEM(Matrix4cd polarisation);
    void receiveUpdatedPolarisationFromSample(Matrix4cd polarisation);
    void receiveUpdatedPolarisationFromPolariser(Matrix4cd polarisation);

private:
    QMutex mutex;
    QWaitCondition condition;
    bool restart, abort;
    int m_numberOfRays;

    std::complex<double> m_q, m_n_1;
    SampleObject *sample;
    PEM *pem;
    PolarisingFilter *polarisingFilter;

    std::vector<CollideableObject*> m_objectsInScene;

    SampleObject* setupSample(std::complex<double> n1, std::complex<double> q);
    PolarisingFilter* setupPolariser(Eigen::Vector2d targetPolarisation);
    PEM *setupPEM(std::complex<double> amplitude, std::complex<double> phase, OGLWidget &representation);
    Matrix4cd generateInitalPolarisation();

    void trace(ListMatrix4cd &outputList, std::vector<CollideableObject*> &objectsInScene, int &rayCount);
    void castRay(Ray &ray, std::vector<CollideableObject*> &objectsInScene, int &depth);
};

#endif // SIMULATIONTHREAD_H
