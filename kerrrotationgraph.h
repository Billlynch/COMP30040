#ifndef KERRROTATIONGRAPH_H
#define KERRROTATIONGRAPH_H

#include <QChart>
#include <QLineSeries>
#include <QSplineSeries>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <complex>
#include <map>
#include <QValueAxis>

typedef std::pair<std::complex<double>, std::complex<double>>
    GraphItem; // p then s
typedef std::map<double, GraphItem> GraphMap;

using namespace QtCharts;

class kerrRotationGraph : public QChartView {
  Q_OBJECT
public:
  explicit kerrRotationGraph(QWidget *parent = nullptr);

private:
  QLineSeries *m_series_p = new QLineSeries();
  QLineSeries *m_series_s = new QLineSeries();
  GraphMap m_graphMap;
  QChart *m_chart = new QChart();

  void addMapToSeries();
  void showChart();

signals:

public slots:
  void updateSeries(GraphMap graphMap);
};

#endif // KERRROTATIONGRAPH_H
