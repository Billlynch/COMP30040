#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <complex>
#include <Core>
#include <QObject>
#include "ray.h"
#include "collideableobject.h"
#include "polarisingfilter.h"
#include "sampleobject.h"
#include <vector>


typedef Eigen::Matrix<std::complex<double>, 2, 1> Vector2cd;
typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 1>> ListVector2cd;
typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2>> ListMatrix4cd;
typedef Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2> Matrix4cd;


class Simulator : public QObject
{
    Q_OBJECT
public:
    Simulator();
    ~Simulator();

    void runSimulation(double Q_r,
                       double Q_i,
                       double n0_r,
                       double n0_i,
                       int rayCount);                       ;

signals:
    void simComplete(ListMatrix4cd polarisations);

public slots:
    void stopSim();

private:
    bool m_stop;
    std::vector<CollideableObject*> objectsInScene;

    SampleObject* setupSample(std::complex<double> n1, std::complex<double> q);
    PolarisingFilter* setupPolariser(Eigen::Vector2d targetPolarisation);

    Matrix4cd generateInitalPolarisation();

    void castRay(Ray &ray ,std::vector<CollideableObject*> objectsInScene, int &depth);
};

#endif // SIMULATOR_H
