#include "sampleobject.h"
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
    SampleObject *sample = new SampleObject(Eigen::Vector3f(0.0f,1.0f,0.0f),
                                            Eigen::Vector3f(0.0f,1.0f,0.0f),
                                            10.0f,
                                            refractiveIndex,
                                            Q,
                                            extinctionCoefficient); // Gold mostly

    std::vector<CollideableObject*> objectsInScene = {sample};

    // this is the base 'image' with the init polarisation
    ListVector2cf out = {Vector2cf(1.0f,1.0f)};


    std::complex<float> Ep = 0.0f;
    std::complex<float> Es = 1.0f;
    Vector2cf polar;
    polar << Ep, Es;

    for (int k = 0; k < rayCount; k++)
    {
        //cast ray from the correct position (origin) - for now all rays are going staight ahead in x
        Eigen::Vector3f rayOrigin = Eigen::Vector3f(1.0f, 0.0f, 0.0f);
        Eigen::Vector3f rayDir = Eigen::Vector3f(-1.0f, 1.0f, 0.0f);

        int depth = 0;

        Ray *ray = new Ray(0.0f, rayOrigin, rayDir, polar, static_cast<double>(waveLength));
        castRay(*ray, objectsInScene, depth);
        out.push_back(ray->getPolarisation());
        delete ray;
    }

    emit Simulator::simComplete(out);
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
