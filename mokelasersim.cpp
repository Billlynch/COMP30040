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
#include <iostream>
#include <qtconcurrentrun.h>
#include <vector>

MOKELaserSim::MOKELaserSim(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MOKELaserSim) {
  ui->setupUi(this);
  resultsWindow = new PolarisationWindow(ui->polarisationVisualisation);
  this->scene = new QGraphicsScene(this);
  this->normalMapImg = new QImage();

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

  setupNormalTargetImage();
  visualisationNormalMapImg->fill(Qt::black);
  renderNormalImage(*visualisationNormalMapImg);

  randomGenerator = new RandomNoiseCalculator(0, 1);
  connect(randomGenerator, &RandomNoiseCalculator::newRandomNoiseGeneration,
          ui->graphicsView, &RandomNoiseChartView::newRandomGenerator);
  connect(randomGenerator, &RandomNoiseCalculator::newRandomNoiseGeneration,
          &thread, &SimulationThread::newLaserNoise);
  randomGenerator->generate();
  ui->graphicsView->setTitle("Laser Noise");

  randomGenerator_pem = new RandomNoiseCalculator(0, 1);
  connect(randomGenerator_pem, &RandomNoiseCalculator::newRandomNoiseGeneration,
          ui->noise_pem, &RandomNoiseChartView::newRandomGenerator);
  connect(randomGenerator_pem, &RandomNoiseCalculator::newRandomNoiseGeneration,
          &thread, &SimulationThread::newPemNoise);
  randomGenerator_pem->generate();
  ui->noise_pem->setTitle("PEM Noise Visualisation");

  randomGenerator_polar = new RandomNoiseCalculator(0, 1);
  connect(randomGenerator_polar,
          &RandomNoiseCalculator::newRandomNoiseGeneration, ui->noise_polar,
          &RandomNoiseChartView::newRandomGenerator);
  connect(randomGenerator_polar,
          &RandomNoiseCalculator::newRandomNoiseGeneration, &thread,
          &SimulationThread::newPolarNoise);
  randomGenerator_polar->generate();
  ui->noise_polar->setTitle("Polariser Noise Visualisation");

  connect(this, &MOKELaserSim::newCameraLocation, ui->threeDVis,
          &ThreeDimentionalVisualisation::newCameraPostion);
  connect(this, &MOKELaserSim::laserNoiseStateChanhe, &thread,
          &SimulationThread::newLaserNoiseState);
  connect(this, &MOKELaserSim::newPemState, &thread,
          &SimulationThread::newPemState);
  connect(this, &MOKELaserSim::newPolariserState, &thread,
          &SimulationThread::newPolariserState);
  connect(this, &MOKELaserSim::newPolarNoiseState, &thread,
          &SimulationThread::newPolarNoiseState);
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

  connect(this, &MOKELaserSim::newNormalFromNormalMap, &thread, &SimulationThread::newNormalFromNormalMap);

}

MOKELaserSim::~MOKELaserSim() {
  delete resultsWindow;
  delete ui;
}

void MOKELaserSim::on_RunSimButton_clicked() {
  thread.simulate(ui->Q_r_input->value(), ui->Q_i_input->value(),
                  ui->n0_r_input->value(), ui->n0_i_input->value(),
                  *ui->kerrGraph, *ui->threeDVis);
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

void MOKELaserSim::on_loadImage_btn_clicked() {
  QString file_path =
      QFileDialog::getOpenFileNames(
          this, tr("Open Normal Map"),
          QStandardPaths::standardLocations(QStandardPaths::DownloadLocation)
              .at(0))
          .at(0);

  std::cout << file_path.toStdString() << std::endl;

  normalMapImg = new QImage(file_path, nullptr);

  renderNormalImage(*normalMapImg);
}

void MOKELaserSim::on_sample_mean_valueChanged(int value) {
  this->randomGenerator->setMean(value / 100.0);
}

void MOKELaserSim::on_sample_deviation_valueChanged(int value) {
  this->randomGenerator->setDeviation(value / 100.0);
}

void MOKELaserSim::on_collisionPointY_valueChanged(int value) {
  this->collisionPoint->setY(value);
  this->updateCollisionVisualisation();
    this->setNormalFromImage();
}

void MOKELaserSim::on_collisionPointX_valueChanged(int value) {
  this->collisionPoint->setX(value);
  this->updateCollisionVisualisation();
    this->setNormalFromImage();
}

void MOKELaserSim::updateCollisionVisualisation() {
  visualisationNormalMapImg = new QImage(*normalMapImg);

  auto *painter = new QPainter(visualisationNormalMapImg);
  painter->drawImage(*collisionPoint, pointImage);
  painter->end();

  renderNormalImage(*visualisationNormalMapImg);
}

void MOKELaserSim::renderNormalImage(QImage &visualisation) {
  scene->addPixmap(QPixmap::fromImage(visualisation));
  scene->setSceneRect(visualisation.rect());

  ui->normalMapView->setScene(scene);
}

void MOKELaserSim::setupNormalTargetImage() {
  QPointF centre = QPointF(10, 10);
  pointImage.fill(Qt::transparent);

  auto *pointImagePainter = new QPainter(&pointImage);
  pointImagePainter->setPen(Qt::red);
  pointImagePainter->setBackgroundMode(Qt::TransparentMode);
  pointImagePainter->drawEllipse(centre, 5, 5);
  QLine hor(10, 5, 10, 15);
  QLine ver(5, 10, 15, 10);
  pointImagePainter->drawLine(hor);
  pointImagePainter->drawLine(ver);
  pointImagePainter->end();
}

void MOKELaserSim::setNormalFromImage() {
  QRgb normalPixel = normalMapImg->pixel(*collisionPoint);

  auto red = convertToVectorScale(qRed(normalPixel));
  auto green = convertToVectorScale(qGreen(normalPixel));
  auto blue = convertToVectorScale(qBlue(normalPixel));

  normalVector = new Eigen::Vector3d(red, green, blue); // y is up in the simulation and z is up in normal maps
  //normalVector->normalize();

  emit newNormalFromNormalMap(*normalVector);
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

void MOKELaserSim::on_deviation_polar_valueChanged(int value) {
  this->randomGenerator_polar->setDeviation(value / 100.0);
}

void MOKELaserSim::on_mean_polar_valueChanged(int value) {
  this->randomGenerator_polar->setMean(value / 100.0);
}

void MOKELaserSim::on_polar_noise_chk_stateChanged(int state) {
  emit newPolarNoiseState(state);
}

void MOKELaserSim::on_my_slider_valueChanged(int value) {
  emit newMyValue(value / 10.0);
}

void MOKELaserSim::on_graph_clear_clicked() { this->ui->kerrGraph->clear(); }
