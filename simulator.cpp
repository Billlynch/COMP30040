//#include "simulator.h"


//Simulator::Simulator()
//{
//}

//Simulator::~Simulator()
//{

//}

//void Simulator::runSimulation(double Q_r, double Q_i, double n0_r, double n0_i, int rayCount)
//{
//    std::complex<double> q = {Q_r, Q_i};
//    std::complex<double> n_1 = {n0_r, n0_i};

//    // setup the sample
//    SampleObject *sample = setupSample(n_1, q);
//    // setup the Polariser
//    PolarisingFilter *polarisingFilter = setupPolariser(Eigen::Vector2d(1.0, 1.0));
//    //setup the PEM
//    PEM *pem = setupPEM(1.0,1.0);

//    objectsInScene = {pem, sample, polarisingFilter};



//    //delete pem;
//    delete sample;
//    delete polarisingFilter;
//}

//void Simulator::run()
//{

//}

//void Simulator::simulation()
//{
//    Matrix4d n;
//    n << 1.0, 0.0, 0.0, 1.0;
//    ListMatrix4cd out = {n};
//    Matrix4cd polar = generateInitalPolarisation();

//    for (int k = 0; k < rayCount; k++)
//    {
//        //cast ray from the correct position (origin) - for now all rays are going staight ahead in x
//        Eigen::Vector3d rayOrigin = Eigen::Vector3d(-2.0, -1.0, 0.0);
//        Eigen::Vector3d rayDir = Eigen::Vector3d(1.0, 1.0, 0.0);

//        int depth = 0;

//        Ray *ray = new Ray(rayOrigin, rayDir, polar, Eigen::Vector2d(1.0, 1.0));
//        castRay(*ray, objectsInScene, depth);
//        ray->setPolarisation( ray->getCalculationMatrix() * polar );
//        out.push_back(ray->getPolarisation());
//        delete ray;
//    }

//    emit Simulator::simComplete(out);
//}


//SampleObject* Simulator::setupSample(std::complex<double> n1, std::complex<double> q)
//{
//   return new SampleObject( Eigen::Vector3d(0.0,2.0,0.0), // location
//                            Eigen::Vector3d(0.0,1.0,0.0), // normal
//                            10.0, // radius
//                            n1, // refractive index
//                            q); // Q value
//}

//PolarisingFilter* Simulator::setupPolariser(Eigen::Vector2d targetPolarisation)
//{
//    return new PolarisingFilter(Eigen::Vector3d(1.0,1.0,0.0),
//                                    Eigen::Vector3d(0.0,1.0,0.0),
//                                    1.0,
//                                    1.0, // no refractive index for now
//                                    targetPolarisation);

//}

//PEM *Simulator::setupPEM(std::complex<double> amplitude, std::complex<double> phase)
//{
//    return new PEM(Eigen::Vector3d(-1.0,1.0,0.0),
//                   Eigen::Vector3d(0.0,1.0,0.0),
//                   1.0,
//                   phase,
//                   amplitude);
//}

//Matrix4cd Simulator::generateInitalPolarisation()
//{
//    std::complex<double> Epp = 1.0;
//    std::complex<double> Esp = 0.0;
//    std::complex<double> Eps = 0.0;
//    std::complex<double> Ess = 1.0;
//    Matrix4cd polar;
//    polar << Epp, Eps, Esp, Ess;

//    return polar;
//}

//void Simulator::castRay(Ray &ray, std::vector<CollideableObject *> objectsInScene, int &depth)
//{
//    if (depth < 5)
//    {
//        depth++;
//        Eigen::Vector3d pointOfInterception = Eigen::Vector3d(0.0, 0.0, 0.0);

//        for (unsigned int j = 0; j < objectsInScene.size(); j++) {
//            if (objectsInScene[j]->intersect(ray, pointOfInterception))
//            {
//                objectsInScene[j]->collide(ray, pointOfInterception);
//                castRay(ray, objectsInScene, depth);
//            }
//        }

//    }
//}
