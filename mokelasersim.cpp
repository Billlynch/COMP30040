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


MOKELaserSim::MOKELaserSim(QWidget* parent) :
  QMainWindow(parent),
  ui(new Ui::MOKELaserSim) {
  ui->setupUi(this);
  resultsWindow = new PolarisationWindow();
  resultsWindow->show();

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

  randomGenerator = new RandomNoiseCalculator(0,1);
  connect(randomGenerator, &RandomNoiseCalculator::newRandomNoiseGeneration, ui->graphicsView, &RandomNoiseChartView::newRandomGenerator);
  randomGenerator->generate();

}

MOKELaserSim::~MOKELaserSim() {
  ui->openGLWidget->close();
  resultsWindow->close();
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

void MOKELaserSim::on_sample_mean_valueChanged(int value)
{
    this->randomGenerator->setMean(value/100.0);
}

void MOKELaserSim::on_sample_deviation_valueChanged(int value)
{
    this->randomGenerator->setDeviation(value/100.0);
}
