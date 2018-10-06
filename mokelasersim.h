#ifndef MOKELASERSIM_H
#define MOKELASERSIM_H

#include <QMainWindow>

namespace Ui {
class MOKELaserSim;
}

class MOKELaserSim : public QMainWindow
{
    Q_OBJECT

public:
    explicit MOKELaserSim(QWidget *parent = nullptr);
    ~MOKELaserSim();

private:
    Ui::MOKELaserSim *ui;
};

#endif // MOKELASERSIM_H
