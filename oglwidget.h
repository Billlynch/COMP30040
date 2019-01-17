#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include "collideableobject.h"

#include <QWidget>
#include <QOpenGLWidget>
#include <eigen3/Eigen/Dense>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <deque>

typedef Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2> Matrix4cd;

const double degreeMulitplier2 = 180.0 / M_PI;


class OGLWidget : public QOpenGLWidget {
  Q_OBJECT
 public:
  OGLWidget(QWidget* parent = nullptr);
  ~OGLWidget() = default;

 private:
  std::deque<Matrix4cd> PEMpolarisations;
  std::deque<Matrix4cd> samplePolarisations;
  std::deque<Matrix4cd> polariserPolarisations;
  std::deque<Matrix4cd> analyserPolarisations;
  Eigen::Vector3d analysierPosition;
  Eigen::Vector3d pemPosition;
  Eigen::Vector3d polarisationPosition;
  Eigen::Vector3d rayDirectionInit;
  Eigen::Vector3d rayDirectionPost;
  std::vector<CollideableObject*> objectsInScene;

  bool readyToRender = false;

  void drawRay(Matrix4cd& polarisaton,  unsigned position, int dir);

  void drawPEM();
  void drawPolariser();
  void drawSample();
  void drawAnalyser();
  void drawObject(CollideableObject& obj);


 protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

 public slots:
  void newOutputFromPEM(Matrix4cd polarisation);
  void newOutputFromPolariser(Matrix4cd polarisation);
  void newOutputFromSample(Matrix4cd polarisation);
  void newOutputFromAnalyser(Matrix4cd polarisation);
  void newPositions(Eigen::Vector3d position,
                    Eigen::Vector3d rayDirection,
                    std::vector<CollideableObject*> objectsInScene);
  void newAngleOfReflection(Eigen::Vector3d angleOfReflection);


};

#endif // OGLWIDGET_H
