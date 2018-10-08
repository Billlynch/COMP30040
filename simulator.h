#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <complex>
#include <Core>
#include <QObject>
#include "ray.h"
#include "collideableobject.h"

typedef Eigen::Matrix<std::complex<float>, 2, 1> Vector2cf;
typedef std::vector<Eigen::Matrix<std::complex<float>, 2, 1>> ListVector2cf;


class Simulator : public QObject
{
    Q_OBJECT
public:
    Simulator();
    ~Simulator();

    void runSimulation(float Q, std::complex<float> refractiveIndex, int rayCount);

signals:
    void simComplete(ListVector2cf polarisations);

public slots:
    void stopSim();

private:
    bool m_stop;

    float castRay(Ray &ray ,std::vector<CollideableObject*> objectsInScene, int &depth);
};

#endif // SIMULATOR_H
