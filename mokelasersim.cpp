#include "mokelasersim.h"
#include "ui_mokelasersim.h"
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
#include <iostream>
#include <qtconcurrentrun.h>
#include <vector>

MOKELaserSim::MOKELaserSim(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MOKELaserSim) {
  ui->setupUi(this);
  resultsWindow = new PolarisationWindow(ui->polarisationVisualisation);

  qRegisterMetaType<ListVector2cd>("ListVector2cd");
  qRegisterMetaType<ListMatrix4cd>("ListMatrix4cd");

  connect(&thread, &SimulationThread::simComplete, resultsWindow,
          &PolarisationWindow::simResultsUpdated);

  eventLoopTimer = new QTimer(this);
  connect(eventLoopTimer, &QTimer::timeout, &thread,
          &SimulationThread::fireNextRay);
  eventLoopTimer->setInterval(2);

  pemTimer = new QTimer(this);
  connect(pemTimer, &QTimer::timeout, &thread,
          &SimulationThread::incrementPEMTimeProgression);
  pemTimer->setInterval(1);

  randomGenerator = new RandomNoiseCalculator(0, 1);
  connect(randomGenerator, &RandomNoiseCalculator::newRandomNoiseGeneration,
          ui->graphicsView, &RandomNoiseChartView::newRandomGenerator);
  connect(randomGenerator, &RandomNoiseCalculator::newRandomNoiseGeneration,
          &thread, &SimulationThread::newLaserNoise);
  randomGenerator->generate();

  randomGenerator_pem = new RandomNoiseCalculator(0, 1);
  connect(randomGenerator_pem, &RandomNoiseCalculator::newRandomNoiseGeneration,
          ui->noise_pem, &RandomNoiseChartView::newRandomGenerator);
  connect(randomGenerator_pem, &RandomNoiseCalculator::newRandomNoiseGeneration,
          &thread, &SimulationThread::newPemNoise);
  randomGenerator_pem->generate();

  connect(this, &MOKELaserSim::newCameraLocation, ui->threeDVis,
          &ThreeDimentionalVisualisation::newCameraPostion);
  connect(this, &MOKELaserSim::laserNoiseStateChanhe, &thread,
          &SimulationThread::newLaserNoiseState);
  connect(this, &MOKELaserSim::newPemState, &thread,
          &SimulationThread::newPemState);
  connect(this, &MOKELaserSim::newPolariserState, &thread,
          &SimulationThread::newPolariserState);
  connect(this, &MOKELaserSim::newPEMNoiseState, &thread,
          &SimulationThread::newPemNoiseState);

  connect(this, &MOKELaserSim::newPolariserState, ui->threeDVis,
          &ThreeDimentionalVisualisation::newPolariserState);

  connect(this, &MOKELaserSim::newPemState, ui->threeDVis,
          &ThreeDimentionalVisualisation::newPemState);

  connect(&thread, &SimulationThread::emittedNewRayFromLaser, ui->threeDVis,
          &ThreeDimentionalVisualisation::newOutputFromLaser);

  connect(this, &MOKELaserSim::newMyValue, &thread,
          &SimulationThread::newMyValue);

  connect(this, &MOKELaserSim::newPolarisationTarget, &thread,
          &SimulationThread::newPolarisationTarget);

  connect(this, &MOKELaserSim::newCoersivity, ui->loop_graph,
          &Loop_graph::updateCoersivity);
}

MOKELaserSim::~MOKELaserSim() {
  delete resultsWindow;
  delete ui;
}

void MOKELaserSim::on_RunSimButton_clicked() {
  thread.simulate(ui->Q_r_input->value(), ui->Q_i_input->value(),
                  ui->n0_r_input->value(), ui->n0_i_input->value(),
                  *ui->kerrGraph, *ui->threeDVis, *ui->loop_graph);
  pemTimer->start();
  eventLoopTimer->start();
}

void MOKELaserSim::on_StopSimButton_clicked() {
  pemTimer->stop();
  eventLoopTimer->stop();
  thread.customAbort();
}

void MOKELaserSim::on_horizontalSlider_valueChanged(int value) {
  pemTimer->setInterval(1 * value);
  eventLoopTimer->setInterval(2 * value);
}

void MOKELaserSim::on_angle_of_incidence_valueChanged(int angle) {
  if (thread.isRunning()) {
    thread.angleOfIncidenceChanged(static_cast<double>(angle));
    this->ui->currentAngle->display(angle);
  }
}

void MOKELaserSim::on_sample_mean_valueChanged(int value) {
  this->randomGenerator->setMean(value / 100.0);
}

void MOKELaserSim::on_sample_deviation_valueChanged(int value) {
  this->randomGenerator->setDeviation(value / 100.0);
}


void MOKELaserSim::on_noise_chk_stateChanged(int state) {
  emit laserNoiseStateChanhe(state);
}

void MOKELaserSim::on_centre_view_clicked() {
  emit newCameraLocation(ViewType::centre);
}

void MOKELaserSim::on_laser_view_clicked() {
  emit newCameraLocation(ViewType::laser);
}

void MOKELaserSim::on_polariser_view_clicked() {
  emit newCameraLocation(ViewType::polarFilter);
}

void MOKELaserSim::on_sample_view_clicked() {
  emit newCameraLocation(ViewType::sample);
}

void MOKELaserSim::on_PEM_view_clicked() {
  emit newCameraLocation(ViewType::pem);
}

void MOKELaserSim::on_Analysier_view_clicked() {
  emit newCameraLocation(ViewType::analyiser);
}

void MOKELaserSim::on_threeDEnable_chk_stateChanged(int state) {
  this->ui->threeDVis->setEnabledState(state);
}

void MOKELaserSim::on_polariEnable_chk_stateChanged(int state) {
  this->resultsWindow->setEnabledState(state);
}

void MOKELaserSim::on_pem_enabled_chk_stateChanged(int state) {
  emit newPemState(state);
}

void MOKELaserSim::on_deviation_pem_valueChanged(int value) {
  this->randomGenerator_pem->setDeviation(value / 100.0);
}

void MOKELaserSim::on_mean_pem_valueChanged(int value) {
  this->randomGenerator_pem->setMean(value / 100.0);
}

void MOKELaserSim::on_polar_enabled_chk_stateChanged(int state) {
  emit newPolariserState(state);
}

void MOKELaserSim::on_my_slider_valueChanged(int value) {
  emit newMyValue(value / 10.0);
}

void MOKELaserSim::on_graph_clear_clicked() { this->ui->kerrGraph->clear(); }

void MOKELaserSim::displayPolarVector(Eigen::Vector2d &target)
{
    target.normalize();
    std::stringstream ss;
    ss << target.x();
    this->ui->polar_s_normalised->setText(QString::fromStdString(ss.str()));
    ss.str(std::string());
    ss << target.y();
    this->ui->polar_p_normalised->setText(QString::fromStdString(ss.str()));
}

void MOKELaserSim::on_polar_direction_valueChanged(int value) {
  this->ui->polar_angle->setText(QString::fromStdString(std::to_string((value + 180) % 360)));
  auto angle = static_cast<double>(value * M_PI / 180.0);

  Eigen::Matrix<double, 2, 2> converter;
  converter << std::cos(angle), std::sin(angle), -std::sin(angle),
      std::cos(angle);

  auto downVector = Eigen::Vector2d(0, -1);

  Eigen::Vector2d target = converter * downVector;

  emit newPolarisationTarget(target);

  displayPolarVector(target);
}

void MOKELaserSim::on_doubleSpinBox_valueChanged(double val) {
  emit newCoersivity(val);
}
