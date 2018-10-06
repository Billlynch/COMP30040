#ifndef MOKELASERSIM_H
#define MOKELASERSIM_H

#include <QDoubleSpinBox>
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

private slots:
    void on_RunSimButton_clicked();

private:
    Ui::MOKELaserSim *ui;

    QDoubleSpinBox *refractiveIndexSpinner;
    QDoubleSpinBox *QSpinner;
};

#endif // MOKELASERSIM_H
