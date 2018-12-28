#include "oglwidget.h"
#include <iostream>

OGLWidget::OGLWidget(QWidget* parent)
  : QOpenGLWidget(parent) {
}

void OGLWidget::drawRay(Matrix4cd& polarisaton, unsigned position, int dir) {
  GLUquadric* quadratic = gluNewQuadric();
  glPushMatrix();
  glTranslated(0.0, 0.0, dir * static_cast<double>(position) * 0.2);
  glColor3f(1.0f, 0.0f, 0.0f);
  glRotatef(90.0f, 1, 0, 0); // set the Cylinder pointing the right way
  glRotatef(static_cast<float>(polarisaton(0, 0).real()) * degreeMulitplier2, 0, 1, 0);
  glRotatef(static_cast<float>(polarisaton(1, 1).real()) * degreeMulitplier2, 0, 0, 1);
  glPushMatrix();
  gluCylinder(quadratic, 0.1, 0.1, 1, 12, 12);
  glTranslatef(0.0f, 0.0f, -0.5f);
  gluCylinder(quadratic, 0.01, 0.2, 0.5, 12, 12);
  glPopMatrix();
  glPopMatrix();
}

void OGLWidget::drawPEM() {
  glPushMatrix();
  glColor3d(0.0, 1.0, 0.0);
  glTranslated(-2.0, 0.0, 0.0);
  glRotated(315.0, 0, 1, 0);
  glutSolidCube(0.4);

  for (unsigned i = 0; i < this->PEMpolarisations.size() - 1; i++) {
    drawRay(this->PEMpolarisations.at(i), i, -1);
  }
  glPopMatrix();
}

void OGLWidget::drawPolariser() {
  glPushMatrix();
  glColor3d(0.0, 1.0, 0.0);
  glTranslated(2.0, 0.0, 0.0);
  glRotated(-315.0, 0, 1, 0);
  glutSolidCube(0.4);

  for (unsigned i = 0; i < this->polariserPolarisations.size() - 1; i++) {
    drawRay(this->polariserPolarisations.at(i), i, 1);
  }
  glPopMatrix();
}

void OGLWidget::drawSample() {
  glPushMatrix();
  glColor3d(0.0, 1.0, 0.0);
  glTranslated(0.0, 10.0, 0.0);
  glutSolidCube(0.4);

  glPushMatrix();
  glRotated(45.0, 0, 1, 0);
  for (unsigned i = 0; i < this->samplePolarisations.size() - 1; i++) {
    drawRay(this->samplePolarisations.at(i), i, 1);
  }
  glPopMatrix();
  glPopMatrix();
}

void OGLWidget::drawAnalyser() {
  glPushMatrix();
  glColor3d(0.0, 1.0, 0.0);
  glTranslated(this->analysierPosition.x(),
               this->analysierPosition.y(),
               this->analysierPosition.z());
  glRotated(315.0, 0, 1, 0);
  glutSolidCube(0.4);

  glPushMatrix();
  for (unsigned i = 0; i < this->analyserPolarisations.size() - 1; i++) {
    drawRay(this->analyserPolarisations.at(i), i, -1);
  }
  glPopMatrix();
  glPopMatrix();
}

void OGLWidget::initializeGL() {
  glClearColor(0, 0, 50, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  Matrix4cd* polarisaton = new Matrix4cd();
  polarisaton->setRandom();
  this->PEMpolarisations.push_back(*polarisaton);
  this->samplePolarisations.push_back(*polarisaton);
  this->polariserPolarisations.push_back(*polarisaton);
  this->analyserPolarisations.push_back(*polarisaton);

}

void OGLWidget::paintGL() {
    if (readyToRender) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//      // X axis (left and right)
//      glPushMatrix();
//          glLineWidth(3.0);

//          glBegin(GL_LINES);
//          //x
//          glColor3f(255, 0, 0);
//          glVertex3f(0, 0, 0);
//          glVertex3f(30, 0, 0);

//          //y
//          glColor3f(0, 255, 0);
//          glVertex3f(0, 0, 0);
//          glVertex3f(0, 30, 0);

//          //z
//          glColor3f(255, 255, 255);
//          glVertex3f(0, 0, 0);
//          glVertex3f(0, 0, 30);
//          glEnd();
//          glLineWidth(1.0);
//       glPopMatrix();

      //drawPEM();
      drawSample();
      //drawPolariser();
      drawAnalyser();

      glFlush();
    }
}

void OGLWidget::newOutputFromPEM(Matrix4cd polarisation) {
  this->PEMpolarisations.insert(this->PEMpolarisations.begin(), polarisation);

  if (this->PEMpolarisations.size() > 11) {
    this->PEMpolarisations.pop_back();
  }
  repaint();
}

void OGLWidget::newOutputFromPolariser(Matrix4cd polarisation) {
  this->polariserPolarisations.insert(this->polariserPolarisations.begin(), polarisation);

  if (this->polariserPolarisations.size() > 11) {
    this->polariserPolarisations.pop_back();
  }
  repaint();
}

void OGLWidget::newOutputFromSample(Matrix4cd polarisation) {
  this->samplePolarisations.insert(this->samplePolarisations.begin(), polarisation);

  if (this->samplePolarisations.size() > 11) {
    this->samplePolarisations.pop_back();
  }
  repaint();
}

void OGLWidget::newOutputFromAnalyser(Matrix4cd polarisation) {
  this->analyserPolarisations.insert(this->analyserPolarisations.begin(), polarisation);

  if (this->analyserPolarisations.size() > 11) {
    this->analyserPolarisations.pop_back();
  }
  repaint();
}

void OGLWidget::newAnalyserPosition(Eigen::Vector3d position)
{
    readyToRender = true;
    std::cout << "new analysise position: "<< std::endl << this->analysierPosition << std::endl;
    this->analysierPosition = position;
}

void OGLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, static_cast<double>(w / h), 0.01, 10000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, -20, 25,
            0, 0, 0,
            0, 0, 1);
}
