#include "simulator.h"


Simulator::Simulator()
{
}

Simulator::~Simulator()
{

}

void Simulator::runSimulation(float Q,
                              std::complex<float> refractiveIndex,
                              int rayCount,
                              double extinctionCoefficient,
                              int waveLength) {

    // draw the scene
    // hard code the properties for now
    std::complex<float> q = {0.0177f,-0.0063f};
    std::complex<float> n_1 = {1.58f,3.58f};


    SampleObject *sample = new SampleObject(Eigen::Vector3f(0.0f,2.0f,0.0f),
                                            Eigen::Vector3f(0.0f,1.0f,0.0f),
                                            10.0f,
                                            n_1,
                                            q,
                                            extinctionCoefficient); // Gold mostly

    PolarisingFilter *polarisingFilter = new PolarisingFilter(Eigen::Vector3f(1.0f,1.0f,0.0f),
                                                              Eigen::Vector3f(0.0f,1.0f,0.0f),
                                                              1.0f,
                                                              1.0f, // no refractive index for now
                                                              Eigen::Vector2cf(1.0f, 1.0f));

    std::vector<CollideableObject*> objectsInScene = {sample, polarisingFilter};

    // this is the base 'image' with the init polarisation
    Matrix4cf n;
    n << 1.0f, 0.0f, 0.0f, 1.0f;
    ListMatrix4cf out = {n};

    std::complex<float> Epp = 1.0f;
    std::complex<float> Esp = 0.0f;
    std::complex<float> Eps = 0.0f;
    std::complex<float> Ess = 1.0f;
    Matrix4cf polar;
    polar << Epp, Eps, Esp, Ess;

    for (int k = 0; k < rayCount; k++)
    {
        //cast ray from the correct position (origin) - for now all rays are going staight ahead in x
        Eigen::Vector3f rayOrigin = Eigen::Vector3f(-2.0f, -1.0f, 0.0f);
        Eigen::Vector3f rayDir = Eigen::Vector3f(1.0f, 1.0f, 0.0f);

        int depth = 0;

        Ray *ray = new Ray(0.0f, rayOrigin, rayDir, polar, Eigen::Vector2cf(1.0f, 1.0f),static_cast<double>(waveLength));
        castRay(*ray, objectsInScene, depth);
        ray->setPolarisation( ray->getCalculationMatrix() * polar );
        out.push_back(ray->getPolarisation());
        delete ray;
    }

    emit Simulator::simComplete(out);
    delete sample;
    delete polarisingFilter;
}

void Simulator::stopSim()
{
    return;
}

void Simulator::castRay(Ray &ray, std::vector<CollideableObject *> objectsInScene, int &depth)
{
    if (depth < 5)
    {
        depth++;
        Eigen::Vector3f pointOfInterception = Eigen::Vector3f(0.0f, 0.0f, 0.0f);

        for (unsigned int j = 0; j < objectsInScene.size(); j++) {
            if (objectsInScene[j]->intersect(ray, pointOfInterception))
            {
                objectsInScene[j]->collide(ray, pointOfInterception);
                castRay(ray, objectsInScene, depth);
            }
        }

    }
}
