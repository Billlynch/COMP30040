#ifndef POLARISATIONWINDOW_H
#define POLARISATIONWINDOW_H

#include <QtGui>
#include <Core>
#include "simulator.h"

typedef Eigen::Matrix<std::complex<float>, 2, 1> Vector2cf;


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
    void simResultsUpdated(std::vector<Vector2cf> polarisations);

protected:
    bool event(QEvent *event);
    void resizeEvent(QResizeEvent *event);
    void exposeEvent(QExposeEvent *event);


private:
    QBackingStore *m_backingStore;
    std::vector<Vector2cf> polarisations;

    void render(QPainter *painter, std::vector<Vector2cf> polarisations);
    void drawAxis(QPainter *painter);
    void drawPolarosations(QPainter *painter, std::vector<Vector2cf> polarisations);
};

#endif // POLARISATIONWINDOW_H
