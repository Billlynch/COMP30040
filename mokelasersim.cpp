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
                  *ui->openGLWidget);
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

void MOKELaserSim::on_horizontalSlider_2_sliderMoved(int position) {
  thread.angleOfIncidenceChanged(static_cast<double>(position));
}
