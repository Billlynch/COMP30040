#include "simulationthread.h"
#include <iostream>

SimulationThread::SimulationThread(QObject *parent) : QThread(parent)
{
    restart = false;
    abort = false;
}

SimulationThread::~SimulationThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();
    wait();

    delete sample;
    delete polarisingFilter;
}

void SimulationThread::customAbort()
{
   this->abort = true;
   condition.wakeOne();
   wait();
   this->abort = false;
}

void SimulationThread::simulate(double Q_r, double Q_i, double n0_r, double n0_i, int rayCount)
{
    QMutexLocker locker(&mutex);

    m_q = {Q_r, Q_i};
    m_n_1 = {n0_r, n0_i};
    m_numberOfRays = rayCount;

    // setup the sample
    sample = setupSample(m_n_1, m_q);
    // setup the Polariser
    polarisingFilter = setupPolariser(Eigen::Vector2d(1.0, 1.0));
    //setup the PEM
    pem = setupPEM(1.0,1.0);

    m_objectsInScene = {pem, sample, polarisingFilter};

    if(!isRunning())
    {
        start(LowPriority);
    } else {
        restart = true;
        condition.wakeOne();
    }
}

SampleObject* SimulationThread::setupSample(std::complex<double> n1, std::complex<double> q)
{
   return new SampleObject( Eigen::Vector3d(0.0,2.0,0.0), // location
                            Eigen::Vector3d(0.0,1.0,0.0), // normal
                            10.0, // radius
                            n1, // refractive index
                            q); // Q value
}

PolarisingFilter* SimulationThread::setupPolariser(Eigen::Vector2d targetPolarisation)
{
    return new PolarisingFilter(Eigen::Vector3d(1.0,1.0,0.0),
                                    Eigen::Vector3d(0.0,1.0,0.0),
                                    1.0,
                                    1.0, // no refractive index for now
                                    targetPolarisation);

}

PEM* SimulationThread::setupPEM(std::complex<double> amplitude, std::complex<double> phase)
{
    return new PEM(Eigen::Vector3d(-1.0,1.0,0.0),
                   Eigen::Vector3d(0.0,1.0,0.0),
                   1.0,
                   phase,
                   amplitude);
}

Matrix4cd SimulationThread::generateInitalPolarisation()
{
    std::complex<double> Epp = 1.0;
    std::complex<double> Esp = 0.0;
    std::complex<double> Eps = 0.0;
    std::complex<double> Ess = 1.0;
    Matrix4cd polar;
    polar << Epp, Eps, Esp, Ess;

    return polar;
}

void SimulationThread::trace(ListMatrix4cd &outputList, std::vector<CollideableObject*> &objectsInScene, int &rayCount)
{
    Matrix4d n;
    n << 1.0, 0.0, 0.0, 1.0;
    outputList = {n};
    Matrix4cd polar = generateInitalPolarisation();

    for (int k = 0; k < rayCount; k++)
    {
        //cast ray from the correct position (origin) - for now all rays are going staight ahead in x
        Eigen::Vector3d rayOrigin = Eigen::Vector3d(-2.0, -1.0, 0.0);
        Eigen::Vector3d rayDir = Eigen::Vector3d(1.0, 1.0, 0.0);

        int depth = 0;

        Ray *ray = new Ray(rayOrigin, rayDir, polar, Eigen::Vector2d(1.0, 1.0));
        castRay(*ray, objectsInScene, depth);
        ray->setPolarisation( ray->getCalculationMatrix() * polar );
        outputList.push_back(ray->getPolarisation());
        delete ray;
    }
}

void SimulationThread::castRay(Ray &ray, std::vector<CollideableObject *> &objectsInScene, int &depth)
{
    if (depth < 5)
    {
        depth++;
        Eigen::Vector3d pointOfInterception = Eigen::Vector3d(0.0, 0.0, 0.0);

        for (unsigned int j = 0; j < objectsInScene.size(); j++) {
            if (objectsInScene[j]->intersect(ray, pointOfInterception))
            {
                objectsInScene[j]->collide(ray, pointOfInterception);
                castRay(ray, objectsInScene, depth);
            }
        }

    }
}

void SimulationThread::incrementPEMTimeProgression()
{
    mutex.lock();
    this->pem->incrementTime();
    std::cout << "Time was incremented" << std::endl;
    mutex.unlock();
}

void SimulationThread::fireNextRay()
{
    mutex.lock();

    ListMatrix4cd outputFromTrace;
    Matrix4d n;
    n << 1.0, 0.0, 0.0, 1.0;
    outputFromTrace = {n};
    Matrix4cd polar = generateInitalPolarisation();

    Eigen::Vector3d rayOrigin = Eigen::Vector3d(-2.0, -1.0, 0.0);
    Eigen::Vector3d rayDir = Eigen::Vector3d(1.0, 1.0, 0.0);

    int depth = 0;

    Ray *ray = new Ray(rayOrigin, rayDir, polar, Eigen::Vector2d(1.0, 1.0));
    castRay(*ray, m_objectsInScene, depth);
    ray->setPolarisation( ray->getCalculationMatrix() * polar );
    outputFromTrace.push_back(ray->getPolarisation());
    delete ray;

    std::cout << "Some output" << std::endl;
    emit simComplete(outputFromTrace);
    mutex.unlock();
}

void SimulationThread::run()
{
    forever {
        mutex.lock();
            restart = false;
            int numberOfRays = this->m_numberOfRays;
            std::vector<CollideableObject *> objectsInScene = this->m_objectsInScene; // shallow copy ok for this as the values will not change atm.
        mutex.unlock();

        ListMatrix4cd outputFromTrace;
        Matrix4d n;
        n << 1.0, 0.0, 0.0, 1.0;
        outputFromTrace = {n};
        Matrix4cd polar = generateInitalPolarisation();

        // actual 'calc'
        for (int rayCount = 0; rayCount < numberOfRays; rayCount++) {
            if (restart)
            {
                break;
            }
            if (abort)
            {
                return;
            }

            //cast ray from the correct position (origin) - for now all rays are going staight ahead in x
            Eigen::Vector3d rayOrigin = Eigen::Vector3d(-2.0, -1.0, 0.0);
            Eigen::Vector3d rayDir = Eigen::Vector3d(1.0, 1.0, 0.0);

            int depth = 0;

            Ray *ray = new Ray(rayOrigin, rayDir, polar, Eigen::Vector2d(1.0, 1.0));
            castRay(*ray, objectsInScene, depth);
            ray->setPolarisation( ray->getCalculationMatrix() * polar );
            outputFromTrace.push_back(ray->getPolarisation());
            delete ray;
            //trace(outputFromTrace, objectsInScene, numberOfRays);
        }



        if (!restart)
        {
            emit simComplete(outputFromTrace);
            return;
        }
    }
}
