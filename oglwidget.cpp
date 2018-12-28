#include "oglwidget.h"
#include <iostream>

OGLWidget::OGLWidget(QWidget* parent)
  : QOpenGLWidget(parent) {

}

void OGLWidget::drawRay(Matrix4cd& polarisaton, unsigned position, int dir) {
  GLUquadric* quadratic = gluNewQuadric();
  //std::cout << "draw! - " << this->polarisations.size() << std::endl;
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

  // draw rays coming from the PEM
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

  // draw rays coming from the Polariser
  for (unsigned i = 0; i < this->polariserPolarisations.size() - 1; i++) {
    drawRay(this->polariserPolarisations.at(i), i, 1);
  }
  glPopMatrix();
}

void OGLWidget::drawSample() {
  glPushMatrix();
  glColor3d(0.0, 1.0, 0.0);
  glTranslated(0.0, 0.0, -2.0);
  glutSolidCube(0.4);

  glPushMatrix();
  glRotated(45.0, 0, 1, 0);
  // draw rays coming from the Sample
  for (unsigned i = 0; i < this->samplePolarisations.size() - 1; i++) {
    drawRay(this->samplePolarisations.at(i), i, 1);
  }
  glPopMatrix();
  glPopMatrix();
}

void OGLWidget::drawAnalyser() {
  glPushMatrix();
  glColor3d(0.0, 1.0, 0.0);
  glTranslated(-4.0, 0.0, 2.0);
  glRotated(315.0, 0, 1, 0);
  glutSolidCube(0.4);

  glPushMatrix();
  // draw rays coming from the Sample
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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  drawPEM();
  drawSample();
  drawPolariser();
  drawAnalyser();

  glFlush();
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

void OGLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, static_cast<double>(w / h), 0.01, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 5, 10,
            0, 0, 0,
            0, 1, 0);
}
