#include "threedimentionalvisualisation.h"
#include <iostream>

ThreeDimentionalVisualisation::ThreeDimentionalVisualisation(QWidget *parent)
   : QWidget(parent)
{
    auto view = new Qt3DExtras::Qt3DWindow();

    // put Qt3DWindow into a container in order to make it possible
    // to handle the view inside a widget
    container = createWindowContainer(view,this);
    container->setFixedSize(QSize(770,500));

   view->defaultFrameGraph()->setClearColor(Qt::gray);

   // Root entity
   rootEntity = new Qt3DCore::QEntity();

   // Camera
   auto cameraEntity = view->camera();
   cameraEntity->setPosition(QVector3D(0, -20, 25));
   cameraEntity->setUpVector(QVector3D(0, 0, 1));
   cameraEntity->setViewCenter(QVector3D(0, 0, 0));

   this->setupSample();
   this->setupPolariser();
   this->setupPEM();
   this->setupLaser();
   this->setupAnalyiser();
   this->setupLineLaserToSample();
   this->setupLineSampleToAnalyiser();

   auto lightEntity = new Qt3DCore::QEntity(rootEntity);
   auto light = new Qt3DRender::QPointLight(lightEntity);
   light->setColor("white");
   light->setIntensity(1.);
   lightEntity->addComponent(light);

   auto lightTransform = new Qt3DCore::QTransform(lightEntity);
   lightTransform->setTranslation(view->camera()->position());
   lightEntity->addComponent(lightTransform);

   // Set root object of the scene
   view->setRootEntity(rootEntity);
}

void ThreeDimentionalVisualisation::setupSample()
{
    // Sample
    auto *modelMesh = new Qt3DRender::QMesh();
    modelMesh->setSource(QUrl(QStringLiteral("qrc:/sample.obj")));

    //Transform
    auto meshTransform = new Qt3DCore::QTransform();
    meshTransform->setScale(0.5f);
    meshTransform->setTranslation(QVector3D(0.0, 10.0, 0.0));
    meshTransform->setRotationZ(90.0f);
    meshTransform->setRotationY(90.0f);

    auto *modelMaterial = new Qt3DExtras::QPhongMaterial();
    modelMaterial->setDiffuse(Qt::red);

    // Model
    auto modelEntity = new Qt3DCore::QEntity(rootEntity);
    modelEntity->addComponent(modelMesh);
    modelEntity->addComponent(meshTransform);
    modelEntity->addComponent(modelMaterial);
}

void ThreeDimentionalVisualisation::setupPolariser()
{
    // Mesh
    auto mesh = new Qt3DRender::QMesh();
    mesh->setSource(QUrl(QStringLiteral("qrc:/filter.obj")));

    //Transform
    PolariserTransform = new Qt3DCore::QTransform();
    PolariserTransform->setScale(0.5f);
    PolariserTransform->setTranslation(QVector3D(1.0, 1.0, 0.0));
    PolariserTransform->setRotationZ(90.0f);
    PolariserTransform->setRotationY(90.0f);
    PolariserTransform->setRotationX(45.0f);


    auto polariserMaterial = new Qt3DExtras::QPhongMaterial();
    polariserMaterial->setDiffuse(Qt::red);

    // Model
    auto modelEntity = new Qt3DCore::QEntity(rootEntity);
    modelEntity->addComponent(mesh);
    modelEntity->addComponent(PolariserTransform);
    modelEntity->addComponent(polariserMaterial);
}

void ThreeDimentionalVisualisation::setupPEM()
{
    // Mesh
    auto mesh = new Qt3DRender::QMesh();
    mesh->setSource(QUrl(QStringLiteral("qrc:/filter.obj")));

    //Transform
    PEMTransform = new Qt3DCore::QTransform();
    PEMTransform->setScale(0.5f);
    PEMTransform->setTranslation(QVector3D(-1.0, 1.0, 0.0));
    PEMTransform->setRotationZ(90.0f);
    PEMTransform->setRotationY(90.0f);
    PEMTransform->setRotationX(-45.0f);


    auto PEMMaterial = new Qt3DExtras::QPhongMaterial();
    PEMMaterial->setDiffuse(Qt::red);

    // Model
    auto modelEntity = new Qt3DCore::QEntity(rootEntity);
    modelEntity->addComponent(mesh);
    modelEntity->addComponent(PEMTransform);
    modelEntity->addComponent(PEMMaterial);
}

void ThreeDimentionalVisualisation::setupLaser()
{
    //Mesh
    auto laserMesh = new Qt3DRender::QMesh();
    laserMesh->setSource(QUrl(QStringLiteral("qrc:/laser.obj")));

    //Transform
    laserTransform = new Qt3DCore::QTransform();
    laserTransform->setScale(0.5f);
    laserTransform->setTranslation(QVector3D(1.0, -5.0, 0.0));
    laserTransform->setRotationZ(90.0f);
    laserTransform->setRotationY(90.0f);
    laserTransform->setRotationX(-45.0f);

    auto *modelMaterial = new Qt3DExtras::QPhongMaterial();
    modelMaterial->setDiffuse(Qt::red);

    // Laser
    auto laserEntity = new Qt3DCore::QEntity(rootEntity);
    laserEntity->addComponent(laserMesh);
    laserEntity->addComponent(laserTransform);
    laserEntity->addComponent(modelMaterial);
}

void ThreeDimentionalVisualisation::setupAnalyiser()
{
    // Mesh
    auto mesh = new Qt3DRender::QMesh();
    mesh->setSource(QUrl(QStringLiteral("qrc:/laser.obj")));

    //Transform
    analysierTransform = new Qt3DCore::QTransform();
    analysierTransform->setScale(0.5f);
    analysierTransform->setTranslation(QVector3D(-1.0, -5.0, 0.0));
    analysierTransform->setRotationZ(90.0f);
    analysierTransform->setRotationY(90.0f);
    analysierTransform->setRotationX(45.0f);


    auto analyiserMaterial = new Qt3DExtras::QPhongMaterial();
    analyiserMaterial->setDiffuse(Qt::red);

    // Model
    auto modelEntity = new Qt3DCore::QEntity(rootEntity);
    modelEntity->addComponent(mesh);
    modelEntity->addComponent(analysierTransform);
    modelEntity->addComponent(analyiserMaterial);
}

void ThreeDimentionalVisualisation::setupLineLaserToSample()
{
    lineLaserToSampleGeometry = new Qt3DRender::QGeometry(rootEntity);

    this->updateLineLaserToSample();
        // mesh
    auto line = new Qt3DRender::QGeometryRenderer(rootEntity);
    line->setGeometry(lineLaserToSampleGeometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    auto *material = new Qt3DExtras::QPhongMaterial(rootEntity);
    material->setAmbient(Qt::green);

    // entity
    auto lineEntity = new Qt3DCore::QEntity(rootEntity);
    lineEntity->addComponent(line);
    lineEntity->addComponent(material);
}

void ThreeDimentionalVisualisation::updateLineLaserToSample()
{
    // position vertices (start and end)
    QByteArray lineBufferBytes;
    lineBufferBytes.resize(3 * 2 * sizeof(float)); // start.x, start.y, start.end + end.x, end.y, end.z
    auto line_laser_to_sample_positions = reinterpret_cast<float*>(lineBufferBytes.data());
    *line_laser_to_sample_positions++ = this->laserPosition.x();
    *line_laser_to_sample_positions++ = this->laserPosition.y();
    *line_laser_to_sample_positions++ = this->laserPosition.z();
    *line_laser_to_sample_positions++ = this->samplePositon.x();
    *line_laser_to_sample_positions++ = this->samplePositon.y();
    *line_laser_to_sample_positions++ = this->samplePositon.z();

    auto buf = new Qt3DRender::QBuffer(lineLaserToSampleGeometry);
    buf->setData(lineBufferBytes);

    auto positionAttribute = new Qt3DRender::QAttribute(lineLaserToSampleGeometry);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(buf);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(2);
    lineLaserToSampleGeometry->addAttribute(positionAttribute); // We add the vertices in the geometry

    // connectivity between vertices
    QByteArray indexBytes;
    indexBytes.resize(2 * sizeof(unsigned int)); // start to end
    auto indices = reinterpret_cast<unsigned int*>(indexBytes.data());
    *indices++ = 0;
    *indices++ = 1;

    auto *indexBuffer = new Qt3DRender::QBuffer(lineLaserToSampleGeometry);
    indexBuffer->setData(indexBytes);

    auto *indexAttribute = new Qt3DRender::QAttribute(lineLaserToSampleGeometry);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(2);
    lineLaserToSampleGeometry->addAttribute(indexAttribute); // We add the indices linking the points in the geometry

}

void ThreeDimentionalVisualisation::setupLineSampleToAnalyiser()
{
    lineSampleToAnalyiserGeometry = new Qt3DRender::QGeometry(rootEntity);

    this->updateLineSampleToAnalyiser();
        // mesh
    auto line = new Qt3DRender::QGeometryRenderer(rootEntity);
    line->setGeometry(lineSampleToAnalyiserGeometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    auto *material = new Qt3DExtras::QPhongMaterial(rootEntity);
    material->setAmbient(Qt::green);

    // entity
    auto lineEntity = new Qt3DCore::QEntity(rootEntity);
    lineEntity->addComponent(line);
    lineEntity->addComponent(material);
}

void ThreeDimentionalVisualisation::updateLineSampleToAnalyiser()
{
    // position vertices (start and end)
    QByteArray lineBufferBytes;
    lineBufferBytes.resize(3 * 2 * sizeof(float)); // start.x, start.y, start.end + end.x, end.y, end.z
    auto line_laser_to_sample_positions = reinterpret_cast<float*>(lineBufferBytes.data());
    *line_laser_to_sample_positions++ = this->samplePositon.x();
    *line_laser_to_sample_positions++ = this->samplePositon.y();
    *line_laser_to_sample_positions++ = this->samplePositon.z();
    *line_laser_to_sample_positions++ = this->analysierPosition.x();
    *line_laser_to_sample_positions++ = this->analysierPosition.y();
    *line_laser_to_sample_positions++ = this->analysierPosition.z();


    auto buf = new Qt3DRender::QBuffer(lineSampleToAnalyiserGeometry);
    buf->setData(lineBufferBytes);

    auto positionAttribute = new Qt3DRender::QAttribute(lineSampleToAnalyiserGeometry);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(buf);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(2);
    lineSampleToAnalyiserGeometry->addAttribute(positionAttribute); // We add the vertices in the geometry

    // connectivity between vertices
    QByteArray indexBytes;
    indexBytes.resize(2 * sizeof(unsigned int)); // start to end
    auto indices = reinterpret_cast<unsigned int*>(indexBytes.data());
    *indices++ = 0;
    *indices++ = 1;

    auto *indexBuffer = new Qt3DRender::QBuffer(lineSampleToAnalyiserGeometry);
    indexBuffer->setData(indexBytes);

    auto *indexAttribute = new Qt3DRender::QAttribute(lineSampleToAnalyiserGeometry);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(2);
    lineSampleToAnalyiserGeometry->addAttribute(indexAttribute); // We add the indices linking the points in the geometry

}

void ThreeDimentionalVisualisation::newPositions(Eigen::Vector3d position, Eigen::Vector3d rayDirection, std::vector<CollideableObject *> objectsInScene)
{
    this->rayDirectionInit = std::move(rayDirection);
    this->analysierPosition = QVector3D(position(0), position(1), position(2));// std::move(position);
    this->objectsInScene = std::move(objectsInScene);

    analysierTransform->setTranslation(this->analysierPosition);
    analysierTransform->setRotationX(-(std::atan(-this->rayDirectionInit.x() / this->rayDirectionInit.y()) * degreeMulitplier3));

    this->laserPosition = QVector3D(this->analysierPosition);
    this->laserPosition.setX(this->analysierPosition.x() * -1.0);
    laserTransform->setRotationX((std::atan(-this->rayDirectionInit.x() / this->rayDirectionInit.y()) * degreeMulitplier3));
    laserTransform->setTranslation(this->laserPosition);

    this->updateLineLaserToSample();
    this->updateLineSampleToAnalyiser();



    foreach (CollideableObject *obj, this->objectsInScene) {
        QVector3D position = QVector3D(obj->getLocation()(0), obj->getLocation()(1), obj->getLocation()(2));

        if (obj->getType() == 1) { // PEM
            PEMTransform->setTranslation(position);
            PEMTransform->setRotationX(-(std::atan(obj->getNormal().x() / obj->getNormal().y()) * degreeMulitplier3) + 90.0);
        } else if (obj->getType() == 2) { // polarising filter
            PolariserTransform->setTranslation(position);
            PolariserTransform->setRotationX(-(std::atan(obj->getNormal().x() / obj->getNormal().y()) * degreeMulitplier3) + 90.0);

        }
    }
}

