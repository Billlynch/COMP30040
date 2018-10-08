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



typedef std::vector<Eigen::Matrix<std::complex<float>, 2, 1>> ListVector2cf;


MOKELaserSim::MOKELaserSim(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MOKELaserSim)
{
    ui->setupUi(this);
    resultsWindow = new PolarisationWindow();
    sim = new Simulator();


    qRegisterMetaType<ListVector2cf>("ListVector2cf");


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
    simThread = QtConcurrent::run(sim, &Simulator::runSimulation, ui->QValueBox->value(),  ui->RefractiveIndexBox->value());
}

void MOKELaserSim::on_StopSimButton_clicked()
{
    emit stopSim();
    simThread.cancel();
    resultsWindow->close();
}
