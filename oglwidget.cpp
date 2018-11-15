#include "oglwidget.h"
#include <iostream>

OGLWidget::OGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

OGLWidget::~OGLWidget()
{

}

void OGLWidget::drawRay(Matrix4cd &polarisaton)
{
    GLUquadric *quadratic = gluNewQuadric();
    std::cout << "draw!" << std::endl;
    glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);
        glRotatef(90.0f, 1, 0, 0); // set the Cylinder pointing the right way
        glRotatef(static_cast<float>(polarisaton(0,0).real()) * degreeMulitplier2 , 0, 1, 0);
        glRotatef(static_cast<float>(polarisaton(1,1).real()) * degreeMulitplier2, 0, 0, 1);
        glPushMatrix();
            gluCylinder(quadratic, 0.1, 0.1, 1, 12, 12);
            glTranslatef(0.0f, 0.0f, -0.5f);
            gluCylinder(quadratic, 0.01, 0.2, 0.5, 12, 12);
        glPopMatrix();
    glPopMatrix();

    glFlush();
}

void OGLWidget::initializeGL()
{
    glClearColor(0,0,50,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    Matrix4cd *polarisaton = new Matrix4cd();
    polarisaton->setRandom();
    this->polarisations.push_back(*polarisaton);
}

void OGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawRay(this->polarisations[this->polarisations.size() - 1]);
}

void OGLWidget::newOutputFromPEM(Matrix4cd polarisation)
{
    this->polarisations.push_back(polarisation);
    repaint();
}

void OGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, static_cast<float>(w/h), 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,5,5,
              0,0,0,
              0,1,0);
}
