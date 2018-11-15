#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <Dense>
#include <glu.h>
#include <gl.h>
#include <GLUT/glut.h>
#include <vector>

typedef Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2> Matrix4cd;

const float degreeMulitplier2 = static_cast<float>(180.0 / M_PI);


class OGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    OGLWidget(QWidget *parent = nullptr);
    ~OGLWidget();

private:
    std::vector<Matrix4cd> polarisations;
    void drawRay(Matrix4cd &polarisaton);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:
    void newOutputFromPEM(Matrix4cd polarisation);
};

#endif // OGLWIDGET_H
