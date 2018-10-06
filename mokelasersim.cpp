#include "mokelasersim.h"
#include "ui_mokelasersim.h"

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
