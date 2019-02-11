#include "threedimentionalvisualisation.h"
#include <iostream>

ThreeDimentionalVisualisation::ThreeDimentionalVisualisation(QWidget *parent)
    : QWidget(parent) {
  if (enabled) {

    auto view = new Qt3DExtras::Qt3DWindow();

    rayMesh->setSource(QUrl(QStringLiteral("qrc:/ray.obj")));

    // put Qt3DWindow into a container in order to make it possible
    // to handle the view inside a widget
    container = createWindowContainer(view, this);
    container->setFixedSize(QSize(770, 500));

    view->defaultFrameGraph()->setClearColor(Qt::gray);

    // Root entity
    rootEntity = new Qt3DCore::QEntity();

    // Camera
    cameraEntity = view->camera();
    cameraEntity->setPosition(QVector3D(0, -20, 25));
    cameraEntity->setUpVector(QVector3D(0, 0, 1));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f,
                                                   1000.0f);

    // For camera controls
    auto *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setLinearSpeed(50.0f);
    camController->setLookSpeed(180.0f);
    camController->setCamera(cameraEntity);

    this->setupSample();
    this->setupPolariser();
    this->setupPEM();
    this->setupLaser();
    this->setupAnalyiser();
    this->setupLineLaserToSample();
    this->setupLineSampleToAnalyiser();
    this->setupLPRays();
    this->setupPARays();
    this->setupPSRays();
    this->setupSPRays();

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
}

void ThreeDimentionalVisualisation::setEnabledState(int state) {
  this->enabled = state;
}

void ThreeDimentionalVisualisation::setupSample() {
  // Sample
  auto *modelMesh = new Qt3DRender::QMesh();
  modelMesh->setSource(QUrl(QStringLiteral("qrc:/sample.obj")));

  // Transform
  auto meshTransform = new Qt3DCore::QTransform();
  meshTransform->setScale(1.0f);
  meshTransform->setTranslation(QVector3D(0.0, 10.0, -3.5));
  meshTransform->setRotationZ(90.0f);
  meshTransform->setRotationY(90.0f);

  auto *modelMaterial = new Qt3DExtras::QPhongMaterial();
  modelMaterial->setDiffuse(Qt::blue);

  // Model
  auto modelEntity = new Qt3DCore::QEntity(rootEntity);
  modelEntity->addComponent(modelMesh);
  modelEntity->addComponent(meshTransform);
  modelEntity->addComponent(modelMaterial);
}

void ThreeDimentionalVisualisation::setupPolariser() {
  // Mesh
  auto mesh = new Qt3DRender::QMesh();
  mesh->setSource(QUrl(QStringLiteral("qrc:/filter.obj")));

  // Transform
  PolariserTransform = new Qt3DCore::QTransform();
  PolariserTransform->setScale(0.5f);
  PolariserTransform->setTranslation(QVector3D(1.0, 1.0, 0.0));
  PolariserTransform->setRotationZ(90.0f);
  PolariserTransform->setRotationY(90.0f);
  PolariserTransform->setRotationX(45.0f);

  polariserMaterial = new Qt3DExtras::QPhongMaterial();
  polariserMaterial->setDiffuse(Qt::black);

  // Model
  auto modelEntity = new Qt3DCore::QEntity(rootEntity);
  modelEntity->addComponent(mesh);
  modelEntity->addComponent(PolariserTransform);
  modelEntity->addComponent(polariserMaterial);
}

void ThreeDimentionalVisualisation::setupPEM() {
  // Mesh
  auto mesh = new Qt3DRender::QMesh();
  mesh->setSource(QUrl(QStringLiteral("qrc:/filter.obj")));

  // Transform
  PEMTransform = new Qt3DCore::QTransform();
  PEMTransform->setScale(0.5f);
  PEMTransform->setTranslation(QVector3D(-1.0, 1.0, 0.0));
  PEMTransform->setRotationZ(90.0f);
  PEMTransform->setRotationY(90.0f);
  PEMTransform->setRotationX(-45.0f);

  PEMMaterial = new Qt3DExtras::QPhongMaterial();
  PEMMaterial->setDiffuse(Qt::black);
  PEMMaterial->setEnabled(this->pemState);

  // Model
  auto modelEntity = new Qt3DCore::QEntity(rootEntity);
  modelEntity->addComponent(mesh);
  modelEntity->addComponent(PEMTransform);
  modelEntity->addComponent(PEMMaterial);
}

void ThreeDimentionalVisualisation::setupLaser() {
  // Mesh
  auto laserMesh = new Qt3DRender::QMesh();
  laserMesh->setSource(QUrl(QStringLiteral("qrc:/laser.obj")));

  // Transform
  laserTransform = new Qt3DCore::QTransform();
  laserTransform->setScale(0.5f);
  laserTransform->setTranslation(QVector3D(1.0, -5.0, 0.0));
  laserTransform->setRotationZ(90.0f);
  laserTransform->setRotationY(90.0f);
  laserTransform->setRotationX(-45.0f);

  auto *modelMaterial = new Qt3DExtras::QPhongMaterial();
  modelMaterial->setDiffuse(Qt::green);

  // Laser
  auto laserEntity = new Qt3DCore::QEntity(rootEntity);
  laserEntity->addComponent(laserMesh);
  laserEntity->addComponent(laserTransform);
  laserEntity->addComponent(modelMaterial);
}

void ThreeDimentionalVisualisation::setupAnalyiser() {
  // Mesh
  auto mesh = new Qt3DRender::QMesh();
  mesh->setSource(QUrl(QStringLiteral("qrc:/laser.obj")));

  // Transform
  analysierTransform = new Qt3DCore::QTransform();
  analysierTransform->setScale(0.5f);
  analysierTransform->setTranslation(QVector3D(-1.0, -5.0, 0.0));
  analysierTransform->setRotationZ(90.0f);
  analysierTransform->setRotationY(90.0f);
  analysierTransform->setRotationX(45.0f);

  auto analyiserMaterial = new Qt3DExtras::QPhongMaterial();
  analyiserMaterial->setDiffuse(Qt::green);

  // Model
  auto modelEntity = new Qt3DCore::QEntity(rootEntity);
  modelEntity->addComponent(mesh);
  modelEntity->addComponent(analysierTransform);
  modelEntity->addComponent(analyiserMaterial);
}

void ThreeDimentionalVisualisation::setupLineLaserToSample() {
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

void ThreeDimentionalVisualisation::updateLineLaserToSample() {
  // position vertices (start and end)
  QByteArray lineBufferBytes;
  lineBufferBytes.resize(
      3 * 2 *
      sizeof(float)); // start.x, start.y, start.end + end.x, end.y, end.z
  auto line_laser_to_sample_positions =
      reinterpret_cast<float *>(lineBufferBytes.data());
  *line_laser_to_sample_positions++ = this->laserPosition.x();
  *line_laser_to_sample_positions++ = this->laserPosition.y();
  *line_laser_to_sample_positions++ = this->laserPosition.z();
  *line_laser_to_sample_positions++ = this->samplePositon.x();
  *line_laser_to_sample_positions++ = this->samplePositon.y();
  *line_laser_to_sample_positions++ = this->samplePositon.z();

  auto buf = new Qt3DRender::QBuffer(lineLaserToSampleGeometry);
  buf->setData(lineBufferBytes);

  auto positionAttribute =
      new Qt3DRender::QAttribute(lineLaserToSampleGeometry);
  positionAttribute->setName(
      Qt3DRender::QAttribute::defaultPositionAttributeName());
  positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
  positionAttribute->setVertexSize(3);
  positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
  positionAttribute->setBuffer(buf);
  positionAttribute->setByteStride(3 * sizeof(float));
  positionAttribute->setCount(2);
  lineLaserToSampleGeometry->addAttribute(
      positionAttribute); // We add the vertices in the geometry

  // connectivity between vertices
  QByteArray indexBytes;
  indexBytes.resize(2 * sizeof(unsigned int)); // start to end
  auto indices = reinterpret_cast<unsigned int *>(indexBytes.data());
  *indices++ = 0;
  *indices++ = 1;

  auto *indexBuffer = new Qt3DRender::QBuffer(lineLaserToSampleGeometry);
  indexBuffer->setData(indexBytes);

  auto *indexAttribute = new Qt3DRender::QAttribute(lineLaserToSampleGeometry);
  indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
  indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
  indexAttribute->setBuffer(indexBuffer);
  indexAttribute->setCount(2);
  lineLaserToSampleGeometry->addAttribute(
      indexAttribute); // We add the indices linking the points in the geometry
}

void ThreeDimentionalVisualisation::setupLineSampleToAnalyiser() {
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

void ThreeDimentionalVisualisation::updateLineSampleToAnalyiser() {
  // position vertices (start and end)
  QByteArray lineBufferBytes;
  lineBufferBytes.resize(
      3 * 2 *
      sizeof(float)); // start.x, start.y, start.end + end.x, end.y, end.z
  auto line_laser_to_sample_positions =
      reinterpret_cast<float *>(lineBufferBytes.data());
  *line_laser_to_sample_positions++ = this->samplePositon.x();
  *line_laser_to_sample_positions++ = this->samplePositon.y();
  *line_laser_to_sample_positions++ = this->samplePositon.z();
  *line_laser_to_sample_positions++ = this->analysierPosition.x();
  *line_laser_to_sample_positions++ = this->analysierPosition.y();
  *line_laser_to_sample_positions++ = this->analysierPosition.z();

  auto buf = new Qt3DRender::QBuffer(lineSampleToAnalyiserGeometry);
  buf->setData(lineBufferBytes);

  auto positionAttribute =
      new Qt3DRender::QAttribute(lineSampleToAnalyiserGeometry);
  positionAttribute->setName(
      Qt3DRender::QAttribute::defaultPositionAttributeName());
  positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
  positionAttribute->setVertexSize(3);
  positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
  positionAttribute->setBuffer(buf);
  positionAttribute->setByteStride(3 * sizeof(float));
  positionAttribute->setCount(2);
  lineSampleToAnalyiserGeometry->addAttribute(
      positionAttribute); // We add the vertices in the geometry

  // connectivity between vertices
  QByteArray indexBytes;
  indexBytes.resize(2 * sizeof(unsigned int)); // start to end
  auto indices = reinterpret_cast<unsigned int *>(indexBytes.data());
  *indices++ = 0;
  *indices++ = 1;

  auto *indexBuffer = new Qt3DRender::QBuffer(lineSampleToAnalyiserGeometry);
  indexBuffer->setData(indexBytes);

  auto *indexAttribute =
      new Qt3DRender::QAttribute(lineSampleToAnalyiserGeometry);
  indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
  indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
  indexAttribute->setBuffer(indexBuffer);
  indexAttribute->setCount(2);
  lineSampleToAnalyiserGeometry->addAttribute(
      indexAttribute); // We add the indices linking the points in the geometry
}

void ThreeDimentionalVisualisation::setupLPRays() {
  LPRaysMaterial = new Qt3DExtras::QPhongMaterial();
  LPRaysMaterial->setDiffuse(Qt::red);
  LPRaysMaterial->setEnabled(false);

  for (int i = 0; i < 10; i++) {
    // default Transform
    auto rayTransfrom = new Qt3DCore::QTransform();
    rayTransfrom->setScale(0.5f);
    rayTransfrom->setTranslation(this->analysierPosition);
    rayTransfrom->setRotationZ(90.0f);
    rayTransfrom->setRotationY(90.0f);
    rayTransfrom->setRotationX(-45.0f);

    // Laser
    auto rayEntity = new Qt3DCore::QEntity(rootEntity);
    rayEntity->addComponent(rayMesh);
    rayEntity->addComponent(rayTransfrom);
    rayEntity->addComponent(LPRaysMaterial);

    this->LPRaysTransforms->insert(this->LPRaysTransforms->begin(),
                                   rayTransfrom);
  }
}

void ThreeDimentionalVisualisation::setupPSRays() {
  PSRaysMaterial = new Qt3DExtras::QPhongMaterial();
  PSRaysMaterial->setDiffuse(Qt::red);
  PSRaysMaterial->setEnabled(false);

  for (int i = 0; i < 10; i++) {
    // default Transform
    auto rayTransfrom = new Qt3DCore::QTransform();
    rayTransfrom->setScale(0.5f);
    rayTransfrom->setTranslation(this->analysierPosition);
    rayTransfrom->setRotationZ(90.0f);
    rayTransfrom->setRotationY(90.0f);
    rayTransfrom->setRotationX(-45.0f);

    // Laser
    auto rayEntity = new Qt3DCore::QEntity(rootEntity);
    rayEntity->addComponent(rayMesh);
    rayEntity->addComponent(rayTransfrom);
    rayEntity->addComponent(PSRaysMaterial);

    this->PSRaysTransforms->insert(this->PSRaysTransforms->begin(),
                                   rayTransfrom);
  }
}

void ThreeDimentionalVisualisation::setupSPRays() {
  SPRaysMaterial = new Qt3DExtras::QPhongMaterial();
  SPRaysMaterial->setDiffuse(Qt::red);
  SPRaysMaterial->setEnabled(false);

  for (int i = 0; i < 10; i++) {
    // default Transform
    auto rayTransfrom = new Qt3DCore::QTransform();
    rayTransfrom->setScale(0.5f);
    rayTransfrom->setTranslation(this->analysierPosition);
    rayTransfrom->setRotationZ(90.0f);
    rayTransfrom->setRotationY(90.0f);
    rayTransfrom->setRotationX(-45.0f);

    // Laser
    auto rayEntity = new Qt3DCore::QEntity(rootEntity);
    rayEntity->addComponent(rayMesh);
    rayEntity->addComponent(rayTransfrom);
    rayEntity->addComponent(SPRaysMaterial);

    this->SPRaysTransforms->insert(this->SPRaysTransforms->begin(),
                                   rayTransfrom);
  }
}

void ThreeDimentionalVisualisation::setupPARays() {
  PARaysMaterial = new Qt3DExtras::QPhongMaterial();
  PARaysMaterial->setDiffuse(Qt::red);
  PARaysMaterial->setEnabled(false);

  for (int i = 0; i < 10; i++) {
    // default Transform
    auto rayTransfrom = new Qt3DCore::QTransform();
    rayTransfrom->setScale(0.5f);
    rayTransfrom->setTranslation(this->analysierPosition);
    rayTransfrom->setRotationZ(90.0f);
    rayTransfrom->setRotationY(90.0f);
    rayTransfrom->setRotationX(-45.0f);

    // Laser
    auto rayEntity = new Qt3DCore::QEntity(rootEntity);
    rayEntity->addComponent(rayMesh);
    rayEntity->addComponent(rayTransfrom);
    rayEntity->addComponent(PARaysMaterial);

    this->PARaysTransforms->insert(this->PARaysTransforms->begin(),
                                   rayTransfrom);
  }
}

void ThreeDimentionalVisualisation::renderRays() {
  // LP

  QVector3D LPPSDirection = this->samplePositon - this->laserPosition;
  LPPSDirection.normalize();
  QVector3D SPPADirection = this->analysierPosition - this->samplePositon;
  SPPADirection.normalize();

  for (unsigned i = 0; i < this->LPRays.size() - 1; i++) {
    this->LPRaysTransforms->at(i)->setRotationY(
        this->LPRays.at(i).getPolarisation()(0, 0).real() * degreeMulitplier3);
    QVector3D position =
        this->laserPosition + (LPPSDirection * (i * RaySpreadFactorLaserSide));
    this->LPRaysTransforms->at(i)->setTranslation(position);
  }

  // PS
  if (!this->PSRays.empty()) {
    this->PSRaysMaterial->setEnabled(true);
    for (unsigned i = 0; i < this->PSRays.size() - 1; i++) {
      this->PSRaysTransforms->at(i)->setRotationY(
          this->PSRays.at(i).getPolarisation()(0, 0).real() *
          degreeMulitplier3);
      QVector3D position =
          (this->PolariserTransform->translation() - filterOffet) +
          (LPPSDirection * ((i-1) * (RaySpreadFactorSampleSide));
      this->PSRaysTransforms->at(i)->setTranslation(position);
    }
  }

  // SP
  if (!this->SPRays.empty()) {
    this->SPRaysMaterial->setEnabled(true);
    for (unsigned i = 0; i < this->SPRays.size() - 1; i++) {
      this->SPRaysTransforms->at(i)->setRotationZ(
          this->SPRays.at(i).getPolarisation()(0, 0).real() *
          degreeMulitplier3);
      QVector3D position = this->samplePositon +
                           (SPPADirection * ((i+1) * RaySpreadFactorSampleSide));
      this->SPRaysTransforms->at(i)->setTranslation(position);
    }
  }

  // PA
  if (!this->PARays.empty()) {
    this->PARaysMaterial->setEnabled(true);
    for (unsigned i = 0; i < this->PARays.size() - 1; i++) {
      this->PARaysTransforms->at(i)->setRotationZ(
          this->PARays.at(i).getPolarisation()(0, 0).real() *
          degreeMulitplier3);
      QVector3D position = (this->PEMTransform->translation() - filterOffet) +
                           (SPPADirection * (i * RaySpreadFactorLaserSide));
      this->PARaysTransforms->at(i)->setTranslation(position);
    }
  }
}

void ThreeDimentionalVisualisation::newPositions(
    Eigen::Vector3d position, Eigen::Vector3d rayDirection,
    std::vector<CollideableObject *> objectsInScene) {
  if (enabled) {

    this->rayDirectionInit = std::move(rayDirection);
    this->analysierPosition = QVector3D(position(0), position(1),
                                        position(2)); // std::move(position);
    this->objectsInScene = std::move(objectsInScene);

    analysierTransform->setTranslation(this->analysierPosition);
    analysierTransform->setRotationX(
        -(std::atan(-this->rayDirectionInit.x() / this->rayDirectionInit.y()) *
          degreeMulitplier3));

    this->laserPosition = QVector3D(this->analysierPosition);
    this->laserPosition.setX(this->analysierPosition.x() * -1.0);
    laserTransform->setRotationX(
        (std::atan(-this->rayDirectionInit.x() / this->rayDirectionInit.y()) *
         degreeMulitplier3));
    laserTransform->setTranslation(this->laserPosition);

    this->updateLineLaserToSample();
    this->updateLineSampleToAnalyiser();

    this->RaySpreadFactorLaserSide =
        this->laserPosition.distanceToPoint(
            this->PolariserTransform->translation()) /
        10.0;
    this->RaySpreadFactorSampleSide =
        this->samplePositon.distanceToPoint(
            this->PolariserTransform->translation()) /
        10.0;

    this->LaserToSampleSpreadFactorLaserSide =
        this->samplePositon.distanceToPoint(this->laserPosition) / 10.0;

    foreach (CollideableObject *obj, this->objectsInScene) {
      QVector3D position = QVector3D(
          obj->getLocation()(0), obj->getLocation()(1), obj->getLocation()(2));

      if (obj->getType() == 1) { // PEM
        PEMTransform->setTranslation(position + filterOffet);
        PEMTransform->setRotationX(
            -(std::atan(obj->getNormal().x() / obj->getNormal().y()) *
              degreeMulitplier3) +
            90.0);
      } else if (obj->getType() == 2) { // polarising filter
        PolariserTransform->setTranslation(position + filterOffet);
        PolariserTransform->setRotationX(
            -(std::atan(obj->getNormal().x() / obj->getNormal().y()) *
              degreeMulitplier3) +
            90.0);
      }
    }
  }
}

void ThreeDimentionalVisualisation::newCameraPostion(ViewType view) {
  if (enabled) {
    switch (view) {
    case centre:
      cameraEntity->setPosition(QVector3D(0, -20, 25));
      cameraEntity->setViewCenter(QVector3D(0, 0, 0));
      break;

    case laser:
      cameraEntity->setPosition(QVector3D(0, -20, 25));
      cameraEntity->setViewCenter(this->laserPosition);
      break;

    case polarFilter:
      cameraEntity->setPosition(QVector3D(0, -20, 25));
      cameraEntity->setViewCenter(this->PolariserTransform->translation());
      break;

    case sample:
      cameraEntity->setPosition(QVector3D(0, -20, 25));
      cameraEntity->setViewCenter(this->samplePositon);
      break;

    case pem:
      cameraEntity->setPosition(QVector3D(0, -20, 25));
      cameraEntity->setViewCenter(this->PEMTransform->translation());
      break;

    case analyiser:
      cameraEntity->setPosition(QVector3D(0, -20, 25));
      cameraEntity->setViewCenter(this->analysierPosition);
      break;

    default:
      cameraEntity->setPosition(QVector3D(0, -20, 25));
      cameraEntity->setViewCenter(QVector3D(0, 0, 0));
      break;
    }
  }
}

void ThreeDimentionalVisualisation::newPemState(int state) {
  this->pemState = state;
  PEMMaterial->setEnabled(state);

  this->PEMToAnalyiserRays.clear();
}

void ThreeDimentionalVisualisation::newPolariserState(int state) {
  this->polariserState = state;
  polariserMaterial->setEnabled(state);
  this->LPRaysMaterial->setEnabled(state);
}

void ThreeDimentionalVisualisation::newOutputFromLaser(Ray ray) {

  if (enabled) {
    LPRaysMaterial->setEnabled(true);

    QVector3D laserToSampleRayDirection =
        this->samplePositon - this->laserPosition;
    laserToSampleRayDirection.normalize();

    this->LPRays.insert(this->LPRays.begin(), std::move(ray));

    if (this->LPRays.size() > 11) {
      auto rayToMoveState = this->LPRays.back();
      if (this->polariserState) {
        rayToMoveState.setPolarisation(
            rayToMoveState.getPolarisation() *
            this->polariserObject->getPolarisationMatrix());
      }
      this->PSRays.insert(this->PSRays.begin(), rayToMoveState);
      this->LPRays.pop_back();
    }

    if (this->PSRays.size() > 11) {
      auto rayToMoveState = this->PSRays.back();
      this->sampleObject->calculatePolarisationUsingGriggsFormulae(
          rayToMoveState, theta0, theta1);

      this->SPRays.insert(this->SPRays.begin(), rayToMoveState);
      this->PSRays.pop_back();
    }

    if (this->SPRays.size() > 11) {
      auto rayToMoveState = this->SPRays.back();
      if (this->pemState) {
        rayToMoveState.setPolarisation(
            rayToMoveState.getPolarisation() *
            this->pemObject->getPolarizationMatrix());
      }
      this->PARays.insert(this->PARays.begin(), rayToMoveState);
      this->SPRays.pop_back();
    }

    if (this->PARays.size() > 11) {
      this->PARays.pop_back();
    }

    this->renderRays();
  }
}

void ThreeDimentionalVisualisation::newThetas(std::complex<double> theta0,
                                              std::complex<double> theta1) {
  this->theta0 = theta0;
  this->theta1 = theta1;
}
