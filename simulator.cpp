#include "sample.h"
#include "simulator.h"
#include <iostream>
#include <vector>
#include <complex>


typedef Eigen::Matrix<std::complex<float>, 2, 1> Vector2cf;
typedef std::vector<Eigen::Matrix<std::complex<float>, 2, 1>> ListVector2cf;


Simulator::Simulator()
{
}

Simulator::~Simulator() {

}


void Simulator::runSimulation(float Q, std::complex<float> refractiveIndex) {

    // draw the scene
    Sample *sample = new Sample(Eigen::Vector3f(1.0f,0.0f,0.0f), Eigen::Vector3f(0.0f,1.0f,0.0f), refractiveIndex, Q); // Gold mostly
    std::vector<CollideableObject*> objectsInScene = {sample};

    // this is the base 'image' with the init polarisation
    ListVector2cf out = {Vector2cf(1.0f,1.0f)};


    std::complex<float> Ep = 0.0f;
    std::complex<float> Es = 1.0f;
    Vector2cf polar;
    polar << Ep, Es;

    for (int k = 0; k < 1; k++) {
        //cast ray from the correct position (origin) - for now all rays are going staight ahead in x
        Eigen::Vector3f rayOrigin = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
        Eigen::Vector3f rayDir = Eigen::Vector3f(1.0f, 1.0f, 0.0f);

        int depth = 0;

        Ray *ray = new Ray(0.0f, rayOrigin, rayDir, polar);
        castRay(*ray, objectsInScene, depth);
        out.push_back(ray->getPolarisation());
        delete ray;
    }

    emit Simulator::simComplete(out);
}

void Simulator::stopSim()
{
    std::cout << "stop pressed" << std::endl;
}

float Simulator::castRay(Ray &ray, std::vector<CollideableObject *> objectsInScene, int &depth)
{
    if (depth < 5)
    {
        depth++;
        Eigen::Vector3f pointOfInterception;

        for (unsigned int j = 0; j < objectsInScene.size(); j ++) {
            if (objectsInScene[j]->intersect(ray, pointOfInterception)){
                objectsInScene[j]->collide(ray, pointOfInterception);
            }
        }
        return castRay(ray, objectsInScene, depth);
    }

    return 0.0f;
}
