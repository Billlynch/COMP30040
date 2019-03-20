#ifndef LOOP_GRAPTH_H
#define LOOP_GRAPTH_H

#include <QChart>
#include <QLineSeries>
#include <QSplineSeries>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <complex>
#include <eigen3/Eigen/Dense>
#include <map>
#include <QValueAxis>

typedef std::pair<double, double> LoopGraphItem; // increase then decrease
typedef std::map<double, LoopGraphItem> LoopGraphMap;

using namespace QtCharts;

class Loop_graph : public QChartView {
  Q_OBJECT
public:
  explicit Loop_graph(QWidget *parent = nullptr);
  ~Loop_graph();

private:
  QLineSeries *m_series_increase = new QLineSeries();
  QLineSeries *m_series_decrease = new QLineSeries();
  LoopGraphMap *m_graphMap = new LoopGraphMap();
  QChart *m_chart = new QChart();
  double *m_coersivity = new double(50);

  void addMapToSeries();
  void showChart();

signals:

public slots:
  void updateCoersivity(double coersivity);
  void updateSeries(Eigen::Vector2cd Er);
};

#endif // LOOP_GRAPTH_H
