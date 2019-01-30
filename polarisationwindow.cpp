#include "polarisationwindow.h"
#include <iostream>
#include <eigen3/Eigen/Core>
#include <QLineF>
#include <QPointF>
#include <QImage>

PolarisationWindow::PolarisationWindow(QGraphicsView *view) {
  this->m_view = view;
  outputImage = new QImage(width, height, QImage::Format_RGB16);
  outputImage->fill(Qt::white);
  render(*outputImage);
  polarisations = ListMatrix4cd();
}


void PolarisationWindow::render(QImage &visualisation) {
    if (enabled) {
  scene = new QGraphicsScene(this);
  scene->addPixmap(QPixmap::fromImage(visualisation));
  scene->setSceneRect(visualisation.rect());
  m_view->setScene(scene);
    }
}

void PolarisationWindow::renderNow()
{
    outputImage = new QImage(width, height, QImage::Format_RGB16);
    outputImage->fill(Qt::white);
    auto *painter = new QPainter(outputImage);
    drawAxis(painter);
    drawPolarosations(painter);
    painter->end();
    render(*outputImage);
}

void PolarisationWindow::simResultsUpdated(ListMatrix4cd &polarisationsIn) {
    if (enabled) {
  polarisations = polarisationsIn;
  renderNow();
    }
}

void PolarisationWindow::setEnabledState(int state)
{
    this->enabled = state;
}

void PolarisationWindow::drawAxis(QPainter* painter) {
    if (enabled) {
  QPointF centreOfWindow(width / 2, height / 2);
  int radiusx = width / 4;
  int radiusy = height / 4;

  painter->drawEllipse(centreOfWindow, radiusx, radiusy );
  // draw the axis
  QPointF left(width / 4, height / 2);
  QPointF right(3 * width / 4, height / 2);
  QPointF top(width / 2, height / 4);
  QPointF bottom(width / 2, 3 * height / 4);
  painter->drawLine(left, right);
  painter->drawLine(top, bottom);
    }
}

void PolarisationWindow::drawPolarosations(QPainter* painter) {
  QPen redPen(QColor("#ff0000"), 2, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
  QPen blackDottedPen(QColor("#000000"), 2, Qt::DotLine, Qt::FlatCap, Qt::RoundJoin);

  Vector2cd centre = Vector2cd(width / 2, height / 2);
  Vector2cd right = Vector2cd(1.0, 0.0);

  QLineF newLine = QLineF();


  for (unsigned int j = 0; j < polarisations.size(); j++) {
    if (j == 0) {
      painter->setPen(redPen);
    }

    if (&polarisations[j] != nullptr) {
        polarisations[j].normalize();

        newLine.setP1(QPointF(static_cast<int>(centre(0).real()),
                              static_cast<int>(centre(1).real())));

        Vector2cd displayPol = Vector2cd(polarisations[j](0, 0), polarisations[j](1, 1));
        auto a = acos(right.dot(displayPol) / right.norm() * displayPol.norm());

        newLine.setAngle(a.real() * degreeMulitplier);
        newLine.setLength(width / 4);

        painter->drawLine(newLine);
    }

    if (j == 0) {
      painter->setPen(blackDottedPen);
    }
  }
}
