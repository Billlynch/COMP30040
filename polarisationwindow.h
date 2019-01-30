#ifndef POLARISATIONWINDOW_H
#define POLARISATIONWINDOW_H

#include <QtGui>
#include <eigen3/Eigen/Core>
#include <QGraphicsView>


typedef Eigen::Matrix<std::complex<double>, 2, 1> Vector2cd;
typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2>> ListMatrix4cd;
typedef Eigen::Matrix<std::complex<double>, 2, 1> Vector2cd;
typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 1>> ListVector2cd;


const float degreeMulitplier = static_cast<float>(180.0 / M_PI);


class PolarisationWindow : public QObject {
  Q_OBJECT
 public:
  explicit PolarisationWindow(QGraphicsView *view);
  virtual ~PolarisationWindow() = default;

 public slots:
  void simResultsUpdated(ListMatrix4cd &polarisations);
  void setEnabledState(int state);

 private:
  int enabled = 1;
  ListMatrix4cd polarisations;
  QImage *outputImage;
  QGraphicsScene *scene;
  int width = 350;
  int height = 350;
  QGraphicsView *m_view;

  void render(QImage &visualisation);
  void renderNow();
  void drawAxis(QPainter* painter);
  void drawPolarosations(QPainter* painter);
};

#endif // POLARISATIONWINDOW_H
