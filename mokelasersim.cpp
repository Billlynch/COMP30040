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


MOKELaserSim::MOKELaserSim(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MOKELaserSim)
{
    ui->setupUi(this);
    resultsWindow = new PolarisationWindow();
    sim = new Simulator();


    qRegisterMetaType<ListVector2cd>("ListVector2cd");
    qRegisterMetaType<ListMatrix4cd>("ListMatrix4cd");


    connect(sim, &Simulator::simComplete,
            resultsWindow, &PolarisationWindow::simResultsUpdated);

    connect(this, &MOKELaserSim::stopSim,
            sim, &Simulator::stopSim);

     resultsWindow->show();
}

MOKELaserSim::~MOKELaserSim()
{
    delete ui;
    delete sim;
}

void MOKELaserSim::on_RunSimButton_clicked()
{
    simThread = QtConcurrent::run(sim, &Simulator::runSimulation,
                                  ui->Q_r_input->value(),
                                  ui->Q_i_input->value(),
                                  ui->n0_r_input->value(),
                                  ui->n0_i_input->value(),
                                  ui->rayCount->value());
    simThread.waitForFinished();
}

void MOKELaserSim::on_StopSimButton_clicked()
{
    emit stopSim();
    simThread.cancel();
    resultsWindow->close();
}
