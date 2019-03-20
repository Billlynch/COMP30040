#include "polarisationwindow.h"
#include <QImage>
#include <QLineF>
#include <QPointF>
#include <eigen3/Eigen/Core>
#include <iostream>
/*!
 * \brief PolarisationWindow::PolarisationWindow
 * \param view - the QT widget to show this inside of in the UI.
 */
PolarisationWindow::PolarisationWindow(QGraphicsView *view) {
  this->m_view = view;
  this->scene = new QGraphicsScene(this);
  outputImage = new QImage(width, height, QImage::Format_RGB16);
  outputImage->fill(Qt::white);
  render(*outputImage);
  polarisations = ListMatrix4cd();
}

/*!
 * \brief PolarisationWindow::render
 * \param visualisation - The image we are rendering the diagram into.
 *
 * This does a simple check to see if this diagram is enabled or not.
 * If it is enabled we convert the input image into a more effcient for showing type
 * then display it in the window.
 */
void PolarisationWindow::render(QImage &visualisation) {
  if (enabled) {
    scene->addPixmap(QPixmap::fromImage(visualisation));
    scene->setSceneRect(visualisation.rect());
    m_view->setScene(scene);
  }
}

/*!
 * \brief PolarisationWindow::renderNow
 *
 * This is called to initialise the rendering process. It starts by clearing the image
 * and setting it to white. Then we setup a brush to 'paint' with. Here we also draw
 * the axis and call the method to draw the polarisaions from the simulation.
 */
void PolarisationWindow::renderNow() {
  delete outputImage;
  outputImage = new QImage(width, height, QImage::Format_RGB16);
  outputImage->fill(Qt::white);
  auto *painter = new QPainter(outputImage);
  drawAxis(painter);
  drawPolarosations(painter);
  painter->end();
  delete painter;
  render(*outputImage);
}

/*!
 * \brief PolarisationWindow::simResultsUpdated
 * \param polarisationsIn - a list of the init polarisation and the output polarisation.
 * This clears the existing list of polarisations and calls render now if the diagram is
 * enabled.
 */
void PolarisationWindow::simResultsUpdated(ListMatrix4cd &polarisationsIn) {
  if (enabled) {
    polarisations.clear();
    polarisations = polarisationsIn;
    renderNow();
  }
}

/*!
 * \brief PolarisationWindow::setEnabledState
 * \param state
 *
 * This updates the enabled state of this diagram.
 */
void PolarisationWindow::setEnabledState(int state) { this->enabled = state; }

/*!
 * \brief PolarisationWindow::drawAxis
 * \param painter
 *
 * Draws the axis on the diagram
 */
void PolarisationWindow::drawAxis(QPainter *painter) {
  if (enabled) {
    QPointF centreOfWindow(width / 2, height / 2);
    int radiusx = width / 4;
    int radiusy = height / 4;

    painter->drawEllipse(centreOfWindow, radiusx, radiusy);
    // draw the axis
    QPointF left(width / 4, height / 2);
    QPointF right(3 * width / 4, height / 2);
    QPointF top(width / 2, height / 4);
    QPointF bottom(width / 2, 3 * height / 4);
    painter->drawLine(left, right);
    painter->drawLine(top, bottom);
  }
}

/*!
 * \brief PolarisationWindow::drawPolarosations
 * \param painter
 *
 * For each polarisation we draw a new line centred on the midle of the circle in the diagram
 * with the length of the circle radius. For the correct angle we look at the anlge between the
 * polarisation vector and the right direction, then draw the line in this angle.
 */
void PolarisationWindow::drawPolarosations(QPainter *painter) {
  QPen redPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
  QPen blackDottedPen(Qt::black, 2, Qt::DotLine, Qt::FlatCap, Qt::RoundJoin);

  Vector2cd centre = Vector2cd(width / 2, height / 2);
  Vector2cd right = Vector2cd(1.0, 0.0);

  QLineF newLine = QLineF();

  for (unsigned int j = 1; j < polarisations.size(); j++) {
    if (j == 1) {
      painter->setPen(redPen);
    }

    if (&polarisations[j] != nullptr) {
      polarisations[j].normalize();

      newLine.setP1(QPointF(static_cast<int>(centre(0).real()),
                            static_cast<int>(centre(1).real())));

      Vector2cd displayPol =
          Vector2cd(polarisations[j](0, 0), polarisations[j](1, 1));
      auto a = acos(right.dot(displayPol) / right.norm() * displayPol.norm());

      newLine.setAngle(a.real() * degreeMulitplier);
      newLine.setLength(width / 4);

      painter->drawLine(newLine);
    }

    if (j == 1) {
      painter->setPen(blackDottedPen);
    }
  }
}
