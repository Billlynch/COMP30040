#ifndef LOOP_GRAPTH_H
#define LOOP_GRAPTH_H

#include <QChart>
#include <QLineSeries>
#include <QSplineSeries>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <complex>
#include <map>
#include <eigen3/Eigen/Dense>

typedef std::pair<double, double> GraphItem; // increase then decrease
typedef std::map<double, GraphItem> GraphMap;

using namespace QtCharts;

class loop_graph : public QChartView {
  Q_OBJECT
public:
  explicit loop_graph(QWidget *parent = nullptr);
  ~loop_graph();

  void clear();

private:
  QLineSeries *m_series_p = new QLineSeries();
  QLineSeries *m_series_s = new QLineSeries();
  GraphMap *m_graphMap = nullptr;
  QChart *m_chart = new QChart();
  double *m_coersivity = nullptr;

  void addMapToSeries();
  void showChart();

signals:

public slots:
  void updateCoersivity(double coersivity);
  void updateSeries(Eigen::Vector2cd Er);
};

#endif // LOOP_GRAPTH_H
