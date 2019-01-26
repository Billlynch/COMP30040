#ifndef MOKELASERSIM_H
#define MOKELASERSIM_H

#include <QDoubleSpinBox>
#include <QMainWindow>
#include <vector>
#include <eigen3/Eigen/Core>
#include <QTimer>
#include "polarisationwindow.h"
#include "simulationthread.h"
#include <QPainter>
#include "randomnoisecalculator.h"
#include "randomnoisechartview.h"


typedef Eigen::Matrix<std::complex<double>, 2, 1> Vector2cd;
typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 1>> ListVector2cd;
typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2>> ListMatrix4cd;
typedef Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2> Matrix4cd;


namespace Ui {
class MOKELaserSim;
}

class MOKELaserSim : public QMainWindow {
  Q_OBJECT

 public:
  explicit MOKELaserSim(QWidget* parent = nullptr);
  ~MOKELaserSim();

  void stopSim();

 private slots:
  void on_RunSimButton_clicked();
  void on_StopSimButton_clicked();


  void on_horizontalSlider_valueChanged(int value);
  void on_angle_of_incidence_valueChanged(int value);

  void on_sample_mean_valueChanged(int value);

  void on_sample_deviation_valueChanged(int value);

  void on_loadImage_btn_clicked();

  void on_collisionPointY_valueChanged(int value);

  void on_collisionPointX_valueChanged(int value);

private:
  Ui::MOKELaserSim* ui;
  PolarisationWindow* resultsWindow;
  SimulationThread thread;
  QTimer* eventLoopTimer;
  QTimer* pemTimer;
  QImage* normalMapImg;
  QImage* visualisationNormalMapImg = new QImage(256,256, QImage::Format_RGBA64);
  QImage pointImage = QImage(20,20, QImage::Format_RGBA64);
  QGraphicsScene* scene;
  QPixmap visableNormalMap;
  QPoint* collisionPoint = new QPoint(0,0);
  Eigen::Vector3f *normalVector = nullptr;
  RandomNoiseCalculator* randomGenerator;
  QImage normalMapImg;

  QDoubleSpinBox* refractiveIndexSpinner;
  QDoubleSpinBox* QSpinner;

  bool loadFile(const QString &fileName);
  void updateCollisionVisualisation();
  void renderNormalImage(QImage &visualisation);
  void setupNormalTargetImage();
  void setNormalFromImage();

};

#endif // MOKELASERSIM_H
