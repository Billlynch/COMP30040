#include "polarisationwindow.h"
#include <iostream>
#include <Core>
#include <QLineF>
#include <QPointF>


PolarisationWindow::PolarisationWindow(QWindow *parent)
    : QWindow(parent)
    , m_backingStore(new QBackingStore(this))
{
    setGeometry(100, 100, 400, 400);
    polarisations = ListMatrix4cf();
}

PolarisationWindow::~PolarisationWindow(){}

bool PolarisationWindow::event(QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest) {
        renderNow();
        return true;
    }
    return QWindow::event(event);
}

void PolarisationWindow::renderLater()
{
    requestUpdate();
}

void PolarisationWindow::resizeEvent(QResizeEvent *resizeEvent)
{
    m_backingStore->resize(resizeEvent->size());
    if (isExposed())
        renderNow();
}

void PolarisationWindow::exposeEvent(QExposeEvent *)
{
    if (isExposed())
        renderNow();
}

void PolarisationWindow::render(QPainter *painter, ListMatrix4cf polarisations)
{
    QPen blackPen(QColor("#000000"), 2, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    painter->setPen(blackPen);
    drawAxis(painter);

    drawPolarosations(painter, polarisations);
}

void PolarisationWindow::renderNow()
{
    if (!isExposed())
        return;

    QRect rect(0, 0, width(), height());
    m_backingStore->beginPaint(rect);

    QPaintDevice *device = m_backingStore->paintDevice();
    QPainter painter(device);

    painter.fillRect(0, 0, width(), height(), Qt::white);
    render(&painter);
    painter.end();

    m_backingStore->endPaint();
    m_backingStore->flush(rect);
}

void PolarisationWindow::simResultsUpdated(ListMatrix4cf polarisationsIn)
{
    polarisations = polarisationsIn;
    renderNow();
}

void PolarisationWindow::render(QPainter *painter)
{
    QPen blackPen(QColor("#000000"), 2, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    painter->setPen(blackPen);
    drawAxis(painter);

    drawPolarosations(painter, polarisations);
}

void PolarisationWindow::drawAxis(QPainter *painter)
{
    painter->drawText(20, 20, "Red line indicates intial polarisation");
    painter->drawText(20, 40, "Black dotted line indicates resulting polarisation");

    QPointF centreOfWindow(width() /2, height() /2);
    int radiusx = width() / 4;
    int radiusy = height() /4;

    painter->drawEllipse(centreOfWindow, radiusx, radiusy );
    // draw the axis
    QPointF left(width() /4, height() /2);
    QPointF right(3 * width() /4, height() /2);
    QPointF top(width() /2, height() /4);
    QPointF bottom(width() /2, 3 * height() /4);
    painter->drawLine(left, right);
    painter->drawLine(top, bottom);
}

void PolarisationWindow::drawPolarosations(QPainter *painter, ListMatrix4cf polarisations)
{
    QPen redPen(QColor("#ff0000"), 2, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    QPen blackDottedPen(QColor("#000000"), 2, Qt::DotLine, Qt::FlatCap, Qt::RoundJoin);

    Vector2cf centre = Vector2cf(width() /2, height() /2);
    Vector2cf right = Vector2cf(1.0f, 0.0f);

    QLineF newLine;


    for (unsigned int j = 0; j < polarisations.size(); j++) {
        if (j == 0)
        {
            painter->setPen(redPen);
        }

        polarisations[j].normalize();

        newLine.setP1(QPointF(static_cast<int>(centre(0).real()),
                              static_cast<int>(centre(1).real())));

        Vector2cf displayPol = Vector2cf(polarisations[j](0,0), polarisations[j](1,1));
        auto a = acos(right.dot(displayPol) / right.norm() * displayPol.norm());

        newLine.setAngle(static_cast<qreal>(a.real()) * degreeMulitplier);
        newLine.setLength(width() / 4);

        painter->drawLine(newLine);

        if (j == 0)
        {
            painter->setPen(blackDottedPen);
        }
    }
}
