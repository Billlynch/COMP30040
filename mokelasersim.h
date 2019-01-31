#ifndef MOKELASERSIM_H
#define MOKELASERSIM_H

#include "polarisationwindow.h"
#include "randomnoisecalculator.h"
#include "randomnoisechartview.h"
#include "simulationthread.h"
#include "viewtype.h"
#include <QDoubleSpinBox>
#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <eigen3/Eigen/Core>
#include <vector>

typedef Eigen::Matrix<std::complex<double>, 2, 1> Vector2cd;
typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 1>> ListVector2cd;
typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2>>
    ListMatrix4cd;
typedef Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2> Matrix4cd;

namespace Ui {
class MOKELaserSim;
}

class MOKELaserSim : public QMainWindow {
  Q_OBJECT

public:
  explicit MOKELaserSim(QWidget *parent = nullptr);
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

  void on_centre_view_clicked();

  void on_laser_view_clicked();

  void on_polariser_view_clicked();

  void on_sample_view_clicked();

  void on_PEM_view_clicked();

  void on_Analysier_view_clicked();

  void on_noise_chk_stateChanged(int arg1);

  void on_threeDEnable_chk_stateChanged(int arg1);

  void on_polariEnable_chk_stateChanged(int arg1);

  void on_pem_enabled_chk_stateChanged(int arg1);

  void on_deviation_pem_valueChanged(int value);

  void on_mean_pem_valueChanged(int value);

  void on_polar_enabled_chk_stateChanged(int arg1);

  void on_deviation_polar_valueChanged(int value);

  void on_mean_polar_valueChanged(int value);


  void on_polar_noise_chk_stateChanged(int state);

  void on_my_slider_valueChanged(int value);

private:
  Ui::MOKELaserSim *ui;
  PolarisationWindow *resultsWindow;
  SimulationThread thread;
  QTimer *eventLoopTimer;
  QTimer *pemTimer;
  QImage *normalMapImg;
  QImage *visualisationNormalMapImg =
      new QImage(256, 256, QImage::Format_RGBA64);
  QImage pointImage = QImage(20, 20, QImage::Format_RGBA64);
  QGraphicsScene *scene;
  QPixmap visableNormalMap;
  QPoint *collisionPoint = new QPoint(0, 0);
  Eigen::Vector3f *normalVector = nullptr;
  RandomNoiseCalculator *randomGenerator = nullptr;
  RandomNoiseCalculator *randomGenerator_pem = nullptr;
  RandomNoiseCalculator *randomGenerator_polar = nullptr;


  bool loadFile(const QString &fileName);
  void updateCollisionVisualisation();
  void renderNormalImage(QImage &visualisation);
  void setupNormalTargetImage();
  void setNormalFromImage();

signals:
  void newCameraLocation(ViewType view);
  void laserNoiseStateChanhe(int state);
  void newPemState(int state);
  void newPolariserState(int state);
  void newPolarNoiseState(int state);
  void newPEMNoiseState(int state);
  void newMyValue(double value);

};

#endif // MOKELASERSIM_H
