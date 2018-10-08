#ifndef MOKELASERSIM_H
#define MOKELASERSIM_H

#include <QDoubleSpinBox>
#include <QMainWindow>
#include <vector>
#include <Core>
#include "polarisationwindow.h"
#include "simulator.h"


typedef Eigen::Matrix<std::complex<float>, 2, 1> Vector2cf;


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
    void on_StopSimButton_clicked();

signals:
    void stopSim();

private:
    Ui::MOKELaserSim *ui;
    PolarisationWindow *resultsWindow;
    Simulator *sim;
    QFuture<void> simThread;

    QDoubleSpinBox *refractiveIndexSpinner;
    QDoubleSpinBox *QSpinner;
};

#endif // MOKELASERSIM_H
