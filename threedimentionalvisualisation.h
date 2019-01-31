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
#include "viewtype.h"

#include <deque>

const double degreeMulitplier3 = 180.0 / M_PI;

class ThreeDimentionalVisualisation : public QWidget {
  Q_OBJECT
  QWidget *container;

public:
  explicit ThreeDimentionalVisualisation(QWidget *parent = nullptr);
  void setEnabledState(int state);

private:
  int enabled = 1;
  int pemState = 1;
  int polariserState = 1;
  Qt3DCore::QEntity *rootEntity;
  std::vector<CollideableObject *> objectsInScene;
  QVector3D analysierPosition;
  QVector3D laserPosition;
  QVector3D samplePositon = QVector3D(0, 10, 0);
  QVector3D filterOffet = QVector3D(0, 0, -3);
  Qt3DRender::QCamera *cameraEntity;

  Eigen::Vector3d pemPosition;
  Eigen::Vector3d polarisationPosition;
  Eigen::Vector3d rayDirectionInit;

  Qt3DCore::QTransform *analysierTransform;
  Qt3DCore::QTransform *PEMTransform;
  Qt3DCore::QTransform *PolariserTransform;
  Qt3DCore::QTransform *laserTransform;

  Qt3DRender::QGeometry *lineLaserToSampleGeometry;
  Qt3DRender::QGeometry *lineSampleToAnalyiserGeometry;


  Qt3DExtras::QPhongMaterial *PEMMaterial;
  Qt3DExtras::QPhongMaterial *polariserMaterial;
  Qt3DExtras::QPhongMaterial *PSRaysMaterial;
  Qt3DExtras::QPhongMaterial *LPRaysMaterial;
  Qt3DExtras::QPhongMaterial *SPRaysMaterial;


  std::deque<Matrix4cd> laserToPolarisingFilterRays;
  std::deque<Matrix4cd> PolarisingFilterToSampleRays;
  std::deque<Matrix4cd> SampleToPEMRays;
  std::deque<Matrix4cd> PEMToAnalyiserRays;

  std::deque<Qt3DCore::QTransform *> *PSRaysTransforms =
      new std::deque<Qt3DCore::QTransform *>;
  std::deque<Qt3DCore::QTransform *> *PolarisingFilterToSampleTransforms =
      new std::deque<Qt3DCore::QTransform *>;
  std::deque<Qt3DCore::QTransform *> *SPRays =
      new std::deque<Qt3DCore::QTransform *>;
  std::deque<Qt3DCore::QTransform *> *PEMToAnalyiserTransforms =
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
  void setupRaysToPolariser();
  void setupRaysToSample();
  void setupRaysToPEM();
  void setupRaysToAnalyiser();

public slots:
  void newPositions(Eigen::Vector3d position, Eigen::Vector3d rayDirection,
                    std::vector<CollideableObject *> objectsInScene);

  void newOutputFromPEM(Matrix4cd polarisation);
  void newOutputFromPolariser(Matrix4cd polarisation);
  void newOutputFromSample(Matrix4cd polarisation);
  void newOutputFromAnalyser(Matrix4cd polarisation);
  void newCameraPostion(ViewType view);
  void newPemState(int state);
  void newPolariserState(int state);
};
#endif // THREEDIMENTIONALVISUALISATION_H
