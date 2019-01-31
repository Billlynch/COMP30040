#ifndef THREEDIMENTIONALVISUALISATION_H
#define THREEDIMENTIONALVISUALISATION_H

#include <QDir>
#include <QObject>
#include <QQuaternion>
#include <QWidget>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPerVertexColorMaterial>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QPointLight>

#include <eigen3/Eigen/Dense>

#include "collideableobject.h"
#include "pem.h"
#include "polarisingfilter.h"
#include "ray.h"
#include "sampleobject.h"
#include "viewtype.h"

#include <deque>

const double degreeMulitplier3 = 180.0 / M_PI;

class ThreeDimentionalVisualisation : public QWidget {
  Q_OBJECT
  QWidget *container;

public:
  explicit ThreeDimentionalVisualisation(QWidget *parent = nullptr);
  void setEnabledState(int state);
  PEM *pemObject;
  SampleObject *sampleObject;
  PolarisingFilter *polariserObject;

private:
  int enabled = 1;
  int pemState = 1;
  int polariserState = 1;

  Qt3DRender::QMesh *rayMesh = new Qt3DRender::QMesh;

  std::complex<double> theta0;
  std::complex<double> theta1;

  Qt3DCore::QEntity *rootEntity;
  std::vector<CollideableObject *> objectsInScene;
  QVector3D analysierPosition;
  QVector3D laserPosition;
  QVector3D samplePositon = QVector3D(0, 10, 0);
  QVector3D filterOffet = QVector3D(0, 0, -3);
  Qt3DRender::QCamera *cameraEntity = nullptr;

  Eigen::Vector3d pemPosition;
  Eigen::Vector3d polarisationPosition;
  Eigen::Vector3d rayDirectionInit;

  Qt3DCore::QTransform *analysierTransform = nullptr;
  Qt3DCore::QTransform *PEMTransform = nullptr;
  Qt3DCore::QTransform *PolariserTransform = nullptr;
  Qt3DCore::QTransform *laserTransform = nullptr;

  Qt3DRender::QGeometry *lineLaserToSampleGeometry = nullptr;
  Qt3DRender::QGeometry *lineSampleToAnalyiserGeometry = nullptr;

  Qt3DExtras::QPhongMaterial *PEMMaterial = nullptr;
  Qt3DExtras::QPhongMaterial *polariserMaterial = nullptr;
  Qt3DExtras::QPhongMaterial *PSRaysMaterial = nullptr;
  Qt3DExtras::QPhongMaterial *LPRaysMaterial = nullptr;
  Qt3DExtras::QPhongMaterial *SPRaysMaterial = nullptr;
  Qt3DExtras::QPhongMaterial *PARaysMaterial = nullptr;


  std::deque<Matrix4cd> laserToPolarisingFilterRays;
  std::deque<Matrix4cd> PolarisingFilterToSampleRays;
  std::deque<Matrix4cd> SampleToPEMRays;
  std::deque<Matrix4cd> PEMToAnalyiserRays;

  std::deque<Ray> LPRays = std::deque<Ray>();
  std::deque<Ray> PSRays = std::deque<Ray>();
  std::deque<Ray> SPRays = std::deque<Ray>();
  std::deque<Ray> PARays = std::deque<Ray>();

  std::deque<Qt3DCore::QTransform *> *LPRaysTransforms =
      new std::deque<Qt3DCore::QTransform *>;
  std::deque<Qt3DCore::QTransform *> *PSRaysTransforms =
      new std::deque<Qt3DCore::QTransform *>;
  std::deque<Qt3DCore::QTransform *> *PolarisingFilterToSampleTransforms =
      new std::deque<Qt3DCore::QTransform *>;
  std::deque<Qt3DCore::QTransform *> *SPRaysTransforms =
      new std::deque<Qt3DCore::QTransform *>;
  std::deque<Qt3DCore::QTransform *> *PEMToAnalyiserTransforms =
      new std::deque<Qt3DCore::QTransform *>;

  std::deque<Qt3DCore::QTransform *> *PARaysTransforms =
      new std::deque<Qt3DCore::QTransform *>;

  float RaySpreadFactorSampleSide = 1.0f;
  float RaySpreadFactorLaserSide = 1.0f;
  float LaserToSampleSpreadFactorLaserSide = 1.0f;
  float LaserToPolariserSpreadFactorLaserSide = 1.0f;

  void setupSample();
  void setupPolariser();
  void setupPEM();
  void setupLaser();
  void setupAnalyiser();
  void drawObject(CollideableObject &obj);
  void setupLineLaserToSample();
  void updateLineLaserToSample();
  void setupLineSampleToAnalyiser();
  void updateLineSampleToAnalyiser();
  void setupLPRays();
  void setupPSRays();
  void setupSPRays();
  void setupPARays();

  void renderRays();


public slots:
  void newPositions(Eigen::Vector3d position, Eigen::Vector3d rayDirection,
                    std::vector<CollideableObject *> objectsInScene);
  void newCameraPostion(ViewType view);
  void newPemState(int state);
  void newPolariserState(int state);
  void newOutputFromLaser(Ray ray);
  void newThetas(std::complex<double> theta0, std::complex<double> theta1);
};
#endif // THREEDIMENTIONALVISUALISATION_H
