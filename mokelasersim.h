#ifndef MOKELASERSIM_H
#define MOKELASERSIM_H

#include <QDoubleSpinBox>
#include <QMainWindow>
#include <vector>
#include <eigen3/Eigen/Core>
#include <QTimer>
#include "polarisationwindow.h"
#include "simulationthread.h"
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

private:
  Ui::MOKELaserSim* ui;
  PolarisationWindow* resultsWindow;
  SimulationThread thread;
  QTimer* eventLoopTimer;
  QTimer* pemTimer;
  QImage* normalMapImg;
  QGraphicsScene *scene;
  QPixmap visableNormalMap;
  RandomNoiseCalculator* randomGenerator;
  QImage normalMapImg;

  QDoubleSpinBox* refractiveIndexSpinner;
  QDoubleSpinBox* QSpinner;

  bool loadFile(const QString &fileName);
};

#endif // MOKELASERSIM_H
