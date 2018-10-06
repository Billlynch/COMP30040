#include "sample.h"
#include "simulator.h"
#include <iostream>
#include <vector>
#include <complex>


Simulator::Simulator()
{

}

Simulator::~Simulator() {

}

float castRay(Ray &ray ,std::vector<CollideableObject*> objectsInScene, int &depth) {
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

    return NULL;
}

void Simulator::runSimulation(float Q, std::complex<float> refractiveIndex) {
    std::cout << "this is run in the sim class now with values: Q " << Q << " Refractive Index: " << refractiveIndex << std::endl;

    // draw the scene
    Sample *sample = new Sample(Eigen::Vector3f(1.0f,0.0f,0.0f), Eigen::Vector3f(0.0f,1.0f,0.0f), refractiveIndex, Q); // Gold mostly
    std::vector<CollideableObject*> objectsInScene = {sample};

    // make a base image
    std::vector<float> pixels = {0, 0 , 0 };


    // for every pixel in the image
    for (unsigned long y = 0; y < pixels.size(); y++) {

        //cast ray from the correct position (origin) - for now all rays are going staight ahead in x
        Eigen::Vector3f rayOrigin = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
        Eigen::Vector3f rayDir = Eigen::Vector3f(1.0f, 1.0f, 0.0f);

        std::complex<float> Ep = 0.0f;
        std::complex<float> Es = 1.0f;
        Vector2cf polar;
        polar << Ep, Es;

        std::cout << "init polar: " << polar << std::endl;

        int depth = 0;

        Ray *ray = new Ray(0.0f, rayOrigin, rayDir, polar);
        castRay(*ray, objectsInScene, depth);

        pixels[y] = ray->getAmplitude();
        std::cout << "after polar: " << ray->getPolarisation() << std::endl;

        delete ray;
    }


    //display the 'image'
    std::cout << "image = { ";
    for (unsigned int i=0; i < pixels.size(); i++)
    {
        std::cout << pixels[i] << " ";
    }
    std::cout << "}" << std::endl;


    delete sample;
}
