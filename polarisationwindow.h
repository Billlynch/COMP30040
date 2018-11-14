#ifndef POLARISATIONWINDOW_H
#define POLARISATIONWINDOW_H

#include <QtGui>
#include <Core>
#include "simulator.h"

typedef Eigen::Matrix<std::complex<double>, 2, 1> Vector2cd;
typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2>> ListMatrix4cd;
typedef Eigen::Matrix<std::complex<double>, 2, 1> Vector2cd;
typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 1>> ListVector2cd;


const float degreeMulitplier = static_cast<float>(180.0 / M_PI);


class PolarisationWindow : public QWindow
{
    Q_OBJECT
public:
    explicit PolarisationWindow(QWindow *parent = nullptr);
    virtual ~PolarisationWindow();
    virtual void render(QPainter *painter);


public slots:
    void renderLater();
    void renderNow();
    void simResultsUpdated(ListMatrix4cd polarisations);

protected:
    bool event(QEvent *event);
    void resizeEvent(QResizeEvent *event);
    void exposeEvent(QExposeEvent *event);


private:
    QBackingStore *m_backingStore;
    ListMatrix4cd polarisations;

    void render(QPainter *painter, ListMatrix4cd polarisations);
    void drawAxis(QPainter *painter);
    void drawPolarosations(QPainter *painter, ListMatrix4cd polarisations);
};

#endif // POLARISATIONWINDOW_H
