#include "mokelasersim.h"
#include "ui_mokelasersim.h"
#include <QDebug>
#include <QThread>
#include <QString>
#include <qtconcurrentrun.h>
#include <QApplication>
#include <QFuture>
#include <vector>
#include <QMetaType>
#include <iostream>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>


MOKELaserSim::MOKELaserSim(QWidget* parent) :
  QMainWindow(parent),
  ui(new Ui::MOKELaserSim) {
  ui->setupUi(this);
  resultsWindow = new PolarisationWindow(ui->polarisationVisualisation);

  qRegisterMetaType<ListVector2cd>("ListVector2cd");
  qRegisterMetaType<ListMatrix4cd>("ListMatrix4cd");


  connect(&thread, &SimulationThread::simComplete,
          resultsWindow, &PolarisationWindow::simResultsUpdated);

  eventLoopTimer = new QTimer(this);
  connect(eventLoopTimer, &QTimer::timeout, &thread, &SimulationThread::fireNextRay);
  eventLoopTimer->setInterval(2);

  pemTimer = new QTimer(this);
  connect(pemTimer, &QTimer::timeout, &thread, &SimulationThread::incrementPEMTimeProgression);
  pemTimer->setInterval(1);

  setupNormalTargetImage();
  visualisationNormalMapImg->fill(Qt::black);
  renderNormalImage(*visualisationNormalMapImg);

  randomGenerator = new RandomNoiseCalculator(0,1);
  connect(randomGenerator, &RandomNoiseCalculator::newRandomNoiseGeneration, ui->graphicsView, &RandomNoiseChartView::newRandomGenerator);
  randomGenerator->generate();


  connect(this, &MOKELaserSim::newNormalFromNormalMap, &thread, &SimulationThread::newNormalFromNormalMap);

}

MOKELaserSim::~MOKELaserSim() {
  ui->openGLWidget->close();
  delete resultsWindow;
  delete ui;
}

void MOKELaserSim::on_RunSimButton_clicked() {
  thread.simulate(ui->Q_r_input->value(),
                  ui->Q_i_input->value(),
                  ui->n0_r_input->value(),
                  ui->n0_i_input->value(),
                  *ui->openGLWidget,
                  *ui->kerrGraph);
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

void MOKELaserSim::on_angle_of_incidence_valueChanged(int angle)
{
    thread.angleOfIncidenceChanged(static_cast<double>(angle));
    this->ui->currentAngle->display(angle);
}



void MOKELaserSim::on_loadImage_btn_clicked()
{
    QString file_path = QFileDialog::getOpenFileNames(this, tr("Open Normal Map"), QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).at(0)).at(0);

    std::cout << file_path.toStdString() << std::endl;

    normalMapImg = new QImage(file_path, nullptr);

    renderNormalImage(*normalMapImg);
}


void MOKELaserSim::on_sample_mean_valueChanged(int value)
{
    this->randomGenerator->setMean(value/100.0);
}

void MOKELaserSim::on_sample_deviation_valueChanged(int value)
{
    this->randomGenerator->setDeviation(value/100.0);
}

void MOKELaserSim::on_collisionPointY_valueChanged(int value)
{
    this->collisionPoint->setY(value);
    this->updateCollisionVisualisation();
}

void MOKELaserSim::on_collisionPointX_valueChanged(int value)
{
    this->collisionPoint->setX(value);
    this->updateCollisionVisualisation();
}

void MOKELaserSim::updateCollisionVisualisation()
{
    visualisationNormalMapImg = new QImage(*normalMapImg);

    auto *painter = new QPainter(visualisationNormalMapImg);
    painter->drawImage(*collisionPoint, pointImage);
    painter->end();

    renderNormalImage(*visualisationNormalMapImg);
    setNormalFromImage();
}

void MOKELaserSim::renderNormalImage(QImage &visualisation)
{
    scene = new QGraphicsScene(this);
    scene->addPixmap(QPixmap::fromImage(visualisation));
    scene->setSceneRect(visualisation.rect());

    ui->normalMapView->setScene(scene);
}

void MOKELaserSim::setupNormalTargetImage()
{
    QPointF centre = QPointF(10,10);
    pointImage.fill(Qt::transparent);

    auto *pointImagePainter = new QPainter(&pointImage);
    pointImagePainter->setPen(Qt::red);
    pointImagePainter->setBackgroundMode(Qt::TransparentMode);
    pointImagePainter->drawEllipse(centre, 5,5);
    QLine hor(10, 5, 10, 15);
    QLine ver(5, 10, 15, 10);
    pointImagePainter->drawLine(hor);
    pointImagePainter->drawLine(ver);
    pointImagePainter->end();
}

void MOKELaserSim::setNormalFromImage()
{
    QRgb normalPixel = normalMapImg->pixel(*collisionPoint);

    normalVector = new Eigen::Vector3d(static_cast<double>(qRed(normalPixel)), static_cast<double>(qGreen(normalPixel)), static_cast<double>(qBlue(normalPixel)));
    normalVector->normalize();
    emit newNormalFromNormalMap(*normalVector);
}
