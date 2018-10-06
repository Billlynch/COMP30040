#include "mokelasersim.h"
#include "ui_mokelasersim.h"
#include <iostream>
#include "simulator.h"
#include <QDebug>
#include <QThread>
#include <QString>
#include <qtconcurrentrun.h>
#include <QApplication>
#include <QFuture>

MOKELaserSim::MOKELaserSim(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MOKELaserSim)
{
    ui->setupUi(this);
}

MOKELaserSim::~MOKELaserSim()
{
    delete ui;
}

void hello(QString name)
{
    qDebug() << "Hello" << name << "from" << QThread::currentThread();
}

void MOKELaserSim::on_RunSimButton_clicked()
{
    Simulator sim = Simulator();

    QFuture<void> simulationThread = QtConcurrent::run(sim, &Simulator::runSimulation, ui->QValueBox->value(),  ui->RefractiveIndexBox->value());

    simulationThread.waitForFinished();

    std::cout << "Sim Run. \n \t with Q: " << ui->QValueBox->value() << std::endl << "\t RefractiveIndex: " << ui->RefractiveIndexBox->value() << std::endl;
}
