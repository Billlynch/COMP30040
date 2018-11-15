//#ifndef SIMULATOR_H
//#define SIMULATOR_H

//#include <complex>
//#include <Core>
//#include <QThread>
//#include "ray.h"
//#include "collideableobject.h"
//#include "polarisingfilter.h"
//#include "sampleobject.h"
//#include "pem.h"
//#include <vector>
//#include <QWaitCondition>


//typedef Eigen::Matrix<std::complex<double>, 2, 1> Vector2cd;
//typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 1>> ListVector2cd;
//typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2>> ListMatrix4cd;
//typedef Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2> Matrix4cd;


//class Simulator : public QThread
//{
//    Q_OBJECT
//public:
//    Simulator();
//    ~Simulator() override;

//    void runSimulation(double Q_r,
//                       double Q_i,
//                       double n0_r,
//                       double n0_i,
//                       int rayCount);

//protected:
//    void run() override;

//signals:
//    void simComplete(ListMatrix4cd polarisations);

//private:
//    bool m_abort, m_restart;
//    int m_rayCount;
//    QMutex mutex;
//    QWaitCondition condition;

//    std::vector<CollideableObject*> objectsInScene;

//    void simulation();

//    SampleObject* setupSample(std::complex<double> n1, std::complex<double> q);
//    PolarisingFilter* setupPolariser(Eigen::Vector2d targetPolarisation);
//    PEM *setupPEM(std::complex<double> amplitude, std::complex<double> phase);

//    Matrix4cd generateInitalPolarisation();

//    void castRay(Ray &ray ,std::vector<CollideableObject*> objectsInScene, int &depth);
//};

//#endif // SIMULATOR_H
