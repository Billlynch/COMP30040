#include "mokelasersim.h"
#include "ui_mokelasersim.h"
#include <iostream>


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

void MOKELaserSim::on_RunSimButton_clicked()
{
    std::cout << "Sim Run. \n \t with Q: " << ui->QValueBox->value() << std::endl << "\t RefractiveIndex: " << ui->RefractiveIndexBox->value() << std::endl;
}
