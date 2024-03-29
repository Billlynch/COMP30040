#ifndef MOKELASERSIM_H
#define MOKELASERSIM_H

#include "polarisationwindow.h"
#include "randomnoisecalculator.h"
#include "randomnoisechartview.h"
#include "simulationthread.h"
#include "objecttype.h"
#include <QDoubleSpinBox>
#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <eigen3/Eigen/Core>
#include <vector>
#include "Loop_graph.h"
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QFuture>
#include <QMessageBox>
#include <QMetaType>
#include <QPixmap>
#include <QString>
#include <QThread>
#include <cmath>
#include <qtconcurrentrun.h>
#include <vector>

typedef Eigen::Matrix<std::complex<double>, 2, 1> Vector2cd;
typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 1>> ListVector2cd;
typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2>>
    ListMatrix4cd;
typedef Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2> Matrix4cd;


namespace Ui {
class MOKELaserSim;
}

/*!
 * \brief The MOKELaserSim class
 * This is the main class which runs the app, this runs in the UI thread and
 * handles all UI interactions. It then coridates the other running threads
 * and sets up the connections between the simulation thread and the visualisations.
 */
class MOKELaserSim : public QMainWindow {
  Q_OBJECT

public:
  explicit MOKELaserSim(QWidget *parent = nullptr);
  ~MOKELaserSim();

  void stopSim();
  
private slots:
  void on_RunSimButton_clicked();
  void on_StopSimButton_clicked();

  void on_speedSlider_valueChanged(int value);
  void on_angle_of_incidence_valueChanged(int value);

  void on_sample_mean_valueChanged(int value);

  void on_sample_deviation_valueChanged(int value);

  void on_centre_view_clicked();

  void on_laser_view_clicked();

  void on_polariser_view_clicked();

  void on_sample_view_clicked();

  void on_PEM_view_clicked();

  void on_Analysier_view_clicked();

  void on_noise_chk_stateChanged(int arg1);

  void on_threeDEnable_chk_stateChanged(int arg1);

  void on_polariserDisplayEnable_chk_stateChanged(int arg1);

  void on_pem_enabled_chk_stateChanged(int arg1);

  void on_deviation_pem_valueChanged(int value);

  void on_mean_pem_valueChanged(int value);

  void on_polar_enabled_chk_stateChanged(int arg1);

  void on_H_slider_valueChanged(int value);

  void on_polar_direction_valueChanged(int value);

  void on_doubleSpinBox_valueChanged(double arg1);

private:
  Ui::MOKELaserSim *ui;
  PolarisationWindow *resultsWindow;
  SimulationThread simThread;
  QTimer *eventLoopTimer;
  QTimer *pemTimer;
  RandomNoiseCalculator *randomGenerator_laser = nullptr;
  RandomNoiseCalculator *randomGenerator_pem = nullptr;

  void displayPolarVector(Eigen::Vector2d &target);

signals:
  void newCameraLocation(ObjectType view);
  void laserNoiseStateChanged(int state);
  void newPemState(int state);
  void newPolariserState(int state);
  void newPEMNoiseState(int state);
  void newHValue(double value);
  void newPolarisationTarget(Eigen::Vector2d target);
  void newCoersivity(double coersivity);

};

#endif // MOKELASERSIM_H
