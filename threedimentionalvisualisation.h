#ifndef THREEDIMENTIONALVISUALISATION_H
#define THREEDIMENTIONALVISUALISATION_H

#include <QObject>
#include <QWidget>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QForwardRenderer>
#include <QQuaternion>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QPerVertexColorMaterial>
#include <Qt3DRender/QPointLight>
#include <QDir>

#include <eigen3/Eigen/Dense>

#include "collideableobject.h"

const double degreeMulitplier3 = 180.0 / M_PI;

class ThreeDimentionalVisualisation : public QWidget
{
    Q_OBJECT
    QWidget *container;
  public:
      explicit ThreeDimentionalVisualisation(QWidget *parent = nullptr);

private:
    Qt3DCore::QEntity *rootEntity;
    std::vector<CollideableObject*> objectsInScene;
    QVector3D analysierPosition;
    QVector3D laserPosition;

    Eigen::Vector3d pemPosition;
    Eigen::Vector3d polarisationPosition;
    Eigen::Vector3d rayDirectionInit;

    Qt3DCore::QTransform *analysierTransform;
    Qt3DCore::QTransform *PEMTransform;
    Qt3DCore::QTransform *PolariserTransform;
    Qt3DCore::QTransform *laserTransform;
    void setupSample();
    void setupPolariser();
    void setupPEM();
    void setupLaser();
    void setupAnalyiser();
    void drawObject(CollideableObject& obj);

public slots:
    void newPositions(Eigen::Vector3d position,
                      Eigen::Vector3d rayDirection,
                      std::vector<CollideableObject*> objectsInScene);


};
#endif // THREEDIMENTIONALVISUALISATION_H
