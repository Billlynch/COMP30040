#include "polarisationwindow.h"
#include <iostream>
#include <Core>
#include <QLineF>
#include <QPointF>

typedef Eigen::Matrix<std::complex<float>, 2, 1> Vector2cf;
typedef std::vector<Eigen::Matrix<std::complex<float>, 2, 1>> ListVector2cf;



PolarisationWindow::PolarisationWindow(QWindow *parent)
    : QWindow(parent)
    , m_backingStore(new QBackingStore(this))
{
    setGeometry(100, 100, 400, 400);
    polarisations = std::vector<Vector2cf>();
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

void PolarisationWindow::render(QPainter *painter, std::vector<Vector2cf> polarisations)
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

void PolarisationWindow::simResultsUpdated(ListVector2cf polarisationsIn)
{
    std::cout << "hey the slot was called 2nd pol: " << polarisationsIn[1].real() << std::endl;

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

void PolarisationWindow::drawPolarosations(QPainter *painter, std::vector<Vector2cf> polarisations)
{
    QPen redPen(QColor("#ff0000"), 2, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    QPen blackDottedPen(QColor("#000000"), 2, Qt::DotLine, Qt::FlatCap, Qt::RoundJoin);

    Vector2cf centre = Vector2cf(width() /2, height() /2);
    Vector2cf right = Vector2cf(1.0f, 0.0f);

    double halfC = 180.0 / M_PI;


    for (unsigned int j = 0; j < polarisations.size(); j++) {
        std::cout << "pos: " << j << std::endl;
        if (j == 0) {
            painter->setPen(redPen);
        }

        std::cout << "point: " << polarisations[j](0).real() << " + " << polarisations[j](0).imag() << std::endl;
        std::cout << "point: " << polarisations[j](1).real() << " + " << polarisations[j](1).imag() << std::endl;

        polarisations[j](1) = polarisations[j](1); // this is needed because in the GUI space y is 0 -> height going down but we want the representation to be height() - 0
        polarisations[j].normalize();

        QLineF newLine;

        newLine.setP1(QPointF(static_cast<int>(centre(0).real()),static_cast<int>(centre(1).real())));

        auto a = acos(right.dot(polarisations[j]) / right.norm() * polarisations[j].norm());

        std::cout << "Angle in rad " << a.real() << std::endl;
        std::cout << "Angle in degrees " << static_cast<qreal>(a.real()) * halfC << std::endl;

        newLine.setAngle(static_cast<qreal>(a.real()) * halfC);
        newLine.setLength(width() / 4);

        painter->drawLine(newLine);

        if (j == 0) {
            painter->setPen(blackDottedPen);
        }
    }
}
